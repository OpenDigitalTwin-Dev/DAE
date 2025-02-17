/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Ioan Sucan */

#include <moveit/rviz_plugin_render_tools/render_shapes.hpp>
#include <moveit/rviz_plugin_render_tools/octomap_render.hpp>
#include <geometric_shapes/check_isometry.h>
#include <geometric_shapes/mesh_operations.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <rviz_rendering/objects/shape.hpp>
#include <ogre_helpers/mesh_shape.hpp>
#include <rviz_common/display_context.hpp>
#include <rviz_default_plugins/robot/robot.hpp>

#include <math.h>
#include <memory>
#include <string>

namespace moveit_rviz_plugin
{
RenderShapes::RenderShapes(rviz_common::DisplayContext* context) : context_(context)
{
}

RenderShapes::~RenderShapes()
{
  clear();
}

void RenderShapes::clear()
{
  scene_shapes_.clear();
  octree_voxel_grids_.clear();
}

void RenderShapes::renderShape(Ogre::SceneNode* node, const shapes::Shape* s, const Eigen::Isometry3d& p,
                               OctreeVoxelRenderMode octree_voxel_rendering, OctreeVoxelColorMode octree_color_mode,
                               const Ogre::ColourValue& color, double alpha)
{
  rviz_rendering::Shape* ogre_shape = nullptr;
  Eigen::Vector3d translation = p.translation();
  Ogre::Vector3 position(translation.x(), translation.y(), translation.z());
  ASSERT_ISOMETRY(p)  // unsanitized input, could contain a non-isometry
  Eigen::Quaterniond q(p.linear());
  Ogre::Quaternion orientation(q.w(), q.x(), q.y(), q.z());

  // we don't know how to render cones directly, but we can convert them to a mesh
  if (s->type == shapes::CONE)
  {
    std::unique_ptr<shapes::Mesh> m(shapes::createMeshFromShape(static_cast<const shapes::Cone&>(*s)));
    if (m)
      renderShape(node, m.get(), p, octree_voxel_rendering, octree_color_mode, color, alpha);
    return;
  }

  switch (s->type)
  {
    case shapes::SPHERE:
    {
      ogre_shape = new rviz_rendering::Shape(rviz_rendering::Shape::Sphere, context_->getSceneManager(), node);
      double d = 2.0 * static_cast<const shapes::Sphere*>(s)->radius;
      ogre_shape->setScale(Ogre::Vector3(d, d, d));
    }
    break;
    case shapes::BOX:
    {
      ogre_shape = new rviz_rendering::Shape(rviz_rendering::Shape::Cube, context_->getSceneManager(), node);
      const double* sz = static_cast<const shapes::Box*>(s)->size;
      ogre_shape->setScale(Ogre::Vector3(sz[0], sz[1], sz[2]));
    }
    break;
    case shapes::CYLINDER:
    {
      ogre_shape = new rviz_rendering::Shape(rviz_rendering::Shape::Cylinder, context_->getSceneManager(), node);
      double d = 2.0 * static_cast<const shapes::Cylinder*>(s)->radius;
      double z = static_cast<const shapes::Cylinder*>(s)->length;
      ogre_shape->setScale(Ogre::Vector3(d, z, d));  // the shape has z as major axis, but the rendered cylinder has y
                                                     // as major axis (assuming z is upright);
    }
    break;
    case shapes::MESH:
    {
      const shapes::Mesh* mesh = static_cast<const shapes::Mesh*>(s);
      if (mesh->triangle_count > 0)
      {
        rviz_rendering::MeshShape* m = new rviz_rendering::MeshShape(context_->getSceneManager(), node);
        ogre_shape = m;

        Ogre::Vector3 normal(0.0, 0.0, 0.0);
        for (unsigned int i = 0; i < mesh->triangle_count; ++i)
        {
          unsigned int i3 = i * 3;
          if (mesh->triangle_normals && !mesh->vertex_normals)
          {
            normal.x = mesh->triangle_normals[i3];
            normal.y = mesh->triangle_normals[i3 + 1];
            normal.z = mesh->triangle_normals[i3 + 2];
          }

          for (int k = 0; k < 3; ++k)
          {
            unsigned int vi = 3 * mesh->triangles[i3 + k];
            Ogre::Vector3 v(mesh->vertices[vi], mesh->vertices[vi + 1], mesh->vertices[vi + 2]);
            if (mesh->vertex_normals)
            {
              Ogre::Vector3 n(mesh->vertex_normals[vi], mesh->vertex_normals[vi + 1], mesh->vertex_normals[vi + 2]);
              m->addVertex(v, n);
            }
            else if (mesh->triangle_normals)
            {
              m->addVertex(v, normal);
            }
            else
            {
              m->addVertex(v);
            }
          }
        }
        m->endTriangles();
      }
    }
    break;

    case shapes::OCTREE:
    {
      if (octree_voxel_rendering != OCTOMAP_DISABLED)
      {
        auto octree = std::make_shared<moveit_rviz_plugin::OcTreeRender>(
            static_cast<const shapes::OcTree*>(s)->octree, octree_voxel_rendering, octree_color_mode, 0u, node);
        octree->setPosition(position);
        octree->setOrientation(orientation);
        octree_voxel_grids_.push_back(octree);
      }
    }
    break;

    default:
      break;
  }

  if (ogre_shape)
  {
    ogre_shape->setColor(color);

    if (s->type == shapes::CYLINDER)
    {
      // in geometric shapes, the z axis of the cylinder is its height;
      // for the rviz shape, the y axis is the height; we add a transform to fix this
      static Ogre::Quaternion fix(Ogre::Radian(M_PI / 2.0), Ogre::Vector3(1.0, 0.0, 0.0));
      orientation = orientation * fix;
    }

    ogre_shape->setPosition(position);
    ogre_shape->setOrientation(orientation);
    scene_shapes_.emplace_back(ogre_shape);
  }
}

void RenderShapes::updateShapeColors(double r, double g, double b, double a)
{
  for (const std::unique_ptr<rviz_rendering::Shape>& shape : scene_shapes_)
    shape->setColor(r, g, b, a);
}

}  // namespace moveit_rviz_plugin

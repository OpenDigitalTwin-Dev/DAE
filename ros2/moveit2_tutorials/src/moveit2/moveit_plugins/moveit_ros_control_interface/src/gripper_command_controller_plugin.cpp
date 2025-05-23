/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, PickNik Inc.
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
 *   * Neither the name of Fraunhofer IPA nor the names of its
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

/* Author: Joseph Schornak */

#include <moveit_ros_control_interface/ControllerHandle.hpp>
#include <pluginlib/class_list_macros.hpp>
#include <moveit_simple_controller_manager/gripper_command_controller_handle.hpp>
#include <rclcpp/node.hpp>
#include <memory>

namespace moveit_ros_control_interface
{
/**
 * \brief Simple allocator for moveit_simple_controller_manager::GripperCommandControllerHandle instances.
 */
class GripperCommandControllerAllocator : public ControllerHandleAllocator
{
public:
  moveit_controller_manager::MoveItControllerHandlePtr alloc(const rclcpp::Node::SharedPtr& node,
                                                             const std::string& name,
                                                             const std::vector<std::string>& /* resources */) override
  {
    return std::make_shared<moveit_simple_controller_manager::GripperCommandControllerHandle>(node, name, "gripper_cmd");
  }
};

}  // namespace moveit_ros_control_interface

PLUGINLIB_EXPORT_CLASS(moveit_ros_control_interface::GripperCommandControllerAllocator,
                       moveit_ros_control_interface::ControllerHandleAllocator);

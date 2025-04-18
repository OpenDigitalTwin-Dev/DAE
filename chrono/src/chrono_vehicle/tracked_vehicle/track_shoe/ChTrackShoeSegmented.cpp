// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Base class for segmented track shoes.
//
// =============================================================================

#include "chrono_vehicle/ChSubsysDefs.h"
#include "chrono_vehicle/tracked_vehicle/track_shoe/ChTrackShoeSegmented.h"

namespace chrono {
namespace vehicle {

ChTrackShoeSegmented::ChTrackShoeSegmented(const std::string& name) : ChTrackShoe(name) {}

void ChTrackShoeSegmented::Initialize(std::shared_ptr<ChBodyAuxRef> chassis,
                                      const ChVector3d& location,
                                      const ChQuaternion<>& rotation) {
    ChTrackShoe::Initialize(chassis, location, rotation);

    m_shoe_sprk_material = m_shoe_sprk_minfo.CreateMaterial(chassis->GetSystem()->GetContactMethod());
}

void ChTrackShoeSegmented::AddVisualizationAssets(VisualizationType vis) {
    if (vis == VisualizationType::NONE)
        return;

    if (m_index == 0) {
        m_geometry.color_spheres = {0.4f, 0.1f, 0.1f};
        m_geometry.color_boxes = {0.6f, 0.3f, 0.3f};
        m_geometry.color_cylinders = {0.4f, 0.1f, 0.1f};
    } else if (m_index % 2 == 0) {
        m_geometry.color_spheres = {0.1f, 0.4f, 0.1f};
        m_geometry.color_boxes = {0.3f, 0.6f, 0.3f};
        m_geometry.color_cylinders = {0.1f, 0.4f, 0.1f};
    } else {
        m_geometry.color_spheres = {0.1f, 0.1f, 0.4f};
        m_geometry.color_boxes = {0.3f, 0.3f, 0.6f};
        m_geometry.color_cylinders = {0.1f, 0.1f, 0.4f};
    }

    m_geometry.CreateVisualizationAssets(m_shoe, vis);
}

void ChTrackShoeSegmented::RemoveVisualizationAssets() {
    ChPart::RemoveVisualizationAssets(m_shoe);
}

}  // end namespace vehicle
}  // end namespace chrono

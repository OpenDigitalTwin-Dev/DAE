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
// Authors: Rainer Gericke
// =============================================================================
//
// HMMWV TMeasy tire subsystem
//
// Updated: 2018-02-24
// =============================================================================

#ifndef U401_TMEASY_TIRE_H
#define U401_TMEASY_TIRE_H

#include "chrono_vehicle/wheeled_vehicle/tire/ChTMeasyTire.h"

#include "chrono_models/ChApiModels.h"

namespace chrono {
namespace vehicle {
namespace unimog {

/// @addtogroup vehicle_models_unimog
/// @{

/// TMeasy tire model for the HMMWV.
class CH_MODELS_API U401_TMeasyTire : public ChTMeasyTire {
  public:
    U401_TMeasyTire(const std::string& name);
    ~U401_TMeasyTire() {}

    virtual double GetVisualizationWidth() const override { return m_width; }

    virtual void SetTMeasyParams() override;
    virtual double GetTireMass() const override { return m_mass; }
    virtual ChVector3d GetTireInertia() const override { return m_inertia; }

    virtual void AddVisualizationAssets(VisualizationType vis) override;
    virtual void RemoveVisualizationAssets() override final;

    void GenerateCharacteristicPlots(const std::string& dirname);

  private:
    static const double m_mass;
    static const ChVector3d m_inertia;

    ChFunctionInterp m_stiffnessMap;

    static const std::string m_meshFile_left;
    static const std::string m_meshFile_right;
    std::shared_ptr<ChVisualShapeTriangleMesh> m_trimesh_shape;
};

/// @} vehicle_models_unimog

}  // namespace unimog
}  // end namespace vehicle
}  // end namespace chrono

#endif
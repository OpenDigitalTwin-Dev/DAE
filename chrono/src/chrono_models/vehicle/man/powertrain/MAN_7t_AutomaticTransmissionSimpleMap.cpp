// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2023 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban, Rainer Gericke
// =============================================================================
//
// Simple automatic transmission to use EngineSimpleMap.
// - no torque converter
// - simple gear-shifting model (in automatic mode)
//
// =============================================================================

#include "chrono_models/vehicle/man/powertrain/MAN_7t_AutomaticTransmissionSimpleMap.h"

namespace chrono {
namespace vehicle {
namespace man {

const double rpm2rads = CH_PI / 30;

MAN_7t_AutomaticTransmissionSimpleMap::MAN_7t_AutomaticTransmissionSimpleMap(const std::string& name)
    : ChAutomaticTransmissionSimpleMap(name) {}

void MAN_7t_AutomaticTransmissionSimpleMap::SetGearRatios(std::vector<double>& fwd, double& rev) {
    rev = -0.089525515;

    fwd.push_back(0.077);
    fwd.push_back(0.118);
    fwd.push_back(0.162);
    fwd.push_back(0.221);
    fwd.push_back(0.283);
    fwd.push_back(0.416);
    fwd.push_back(0.573);
    fwd.push_back(0.780);
    fwd.push_back(1.0);
}

void MAN_7t_AutomaticTransmissionSimpleMap::SetShiftPoints(std::vector<std::pair<double, double>>& shift_bands) {
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
    shift_bands.push_back(std::pair<double, double>(1000 * rpm2rads, 2200 * rpm2rads));
}

}  // namespace man
}  // end namespace vehicle
}  // end namespace chrono

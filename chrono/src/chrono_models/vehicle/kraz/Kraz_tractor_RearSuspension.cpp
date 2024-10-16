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
// Authors: Rainer Gericke, Radu Serban
// =============================================================================
//
// Rear Kraz 64431 suspension subsystems (simple leafspring work-a-like).
//
// These concrete suspension subsystems are defined with respect to right-handed
// frames with X pointing towards the front, Y to the left, and Z up.
//
// All point locations are provided for the left half of the suspension.
//
// =============================================================================

#include "chrono_models/vehicle/kraz/Kraz_tractor_RearSuspension.h"

namespace chrono {
namespace vehicle {
namespace kraz {

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------

static const double in2m = 0.0254;

const double Kraz_tractor_RearSuspension::m_axleTubeMass = 717.0;
const double Kraz_tractor_RearSuspension::m_spindleMass = 14.705;

const double Kraz_tractor_RearSuspension::m_axleTubeRadius = 0.06;
const double Kraz_tractor_RearSuspension::m_spindleRadius = 0.10;
const double Kraz_tractor_RearSuspension::m_spindleWidth = 0.06;

const ChVector3d Kraz_tractor_RearSuspension::m_axleTubeInertia(240.8417938, 1.2906, 240.8417938);
const ChVector3d Kraz_tractor_RearSuspension::m_spindleInertia(0.04117, 0.07352, 0.04117);

const double Kraz_tractor_RearSuspension::m_springDesignLength = 0.2;
const double Kraz_tractor_RearSuspension::m_springCoefficient = 1026438.858;
const double Kraz_tractor_RearSuspension::m_springRestLength = m_springDesignLength + 0.062122551;
const double Kraz_tractor_RearSuspension::m_springMinLength = m_springDesignLength - 0.08;
const double Kraz_tractor_RearSuspension::m_springMaxLength = m_springDesignLength + 0.08;
const double Kraz_tractor_RearSuspension::m_damperCoefficient = 163362.818;
const double Kraz_tractor_RearSuspension::m_damperDegressivityCompression = 3.0;
const double Kraz_tractor_RearSuspension::m_damperDegressivityExpansion = 1.0;
const double Kraz_tractor_RearSuspension::m_axleShaftInertia = 0.4;

const double Kraz_tractor_RearSuspension::m_twin_tire_dist = 12.0 * in2m * 1.1;

// ---------------------------------------------------------------------------------------
// Tractor spring functor class - implements a linear spring + bump stop + rebound stop
// ---------------------------------------------------------------------------------------
class Tractor_SpringForceRear : public ChLinkTSDA::ForceFunctor {
  public:
    Tractor_SpringForceRear(double spring_constant, double min_length, double max_length);

    virtual double evaluate(double time,
                            double rest_length,
                            double length,
                            double vel,
                            const ChLinkTSDA& link) override;

  private:
    double m_spring_constant;
    double m_min_length;
    double m_max_length;

    ChFunctionInterp m_bump;
};

Tractor_SpringForceRear::Tractor_SpringForceRear(double spring_constant, double min_length, double max_length)
    : m_spring_constant(spring_constant), m_min_length(min_length), m_max_length(max_length) {
    // From ADAMS/Car
    m_bump.AddPoint(0.0, 0.0);
    m_bump.AddPoint(2.0e-3, 200.0);
    m_bump.AddPoint(4.0e-3, 400.0);
    m_bump.AddPoint(6.0e-3, 600.0);
    m_bump.AddPoint(8.0e-3, 800.0);
    m_bump.AddPoint(10.0e-3, 1000.0);
    m_bump.AddPoint(20.0e-3, 2500.0);
    m_bump.AddPoint(30.0e-3, 4500.0);
    m_bump.AddPoint(40.0e-3, 7500.0);
    m_bump.AddPoint(50.0e-3, 12500.0);
}

double Tractor_SpringForceRear::evaluate(double time,
                                         double rest_length,
                                         double length,
                                         double vel,
                                         const ChLinkTSDA& link) {
    double force = 0;

    double defl_spring = rest_length - length;
    double defl_bump = 0.0;
    double defl_rebound = 0.0;

    if (length < m_min_length) {
        defl_bump = m_min_length - length;
    }

    if (length > m_max_length) {
        defl_rebound = length - m_max_length;
    }

    force = defl_spring * m_spring_constant + m_bump.GetVal(defl_bump) - m_bump.GetVal(defl_rebound);

    return force;
}

// -----------------------------------------------------------------------------
// Tractor shock functor class - implements a nonlinear damper
// -----------------------------------------------------------------------------
class Tractor_ShockForceRear : public ChLinkTSDA::ForceFunctor {
  public:
    Tractor_ShockForceRear(double compression_slope,
                           double compression_degressivity,
                           double expansion_slope,
                           double expansion_degressivity);

    virtual double evaluate(double time,
                            double rest_length,
                            double length,
                            double vel,
                            const ChLinkTSDA& link) override;

  private:
    double m_slope_compr;
    double m_slope_expand;
    double m_degres_compr;
    double m_degres_expand;
};

Tractor_ShockForceRear::Tractor_ShockForceRear(double compression_slope,
                                               double compression_degressivity,
                                               double expansion_slope,
                                               double expansion_degressivity)
    : m_slope_compr(compression_slope),
      m_degres_compr(compression_degressivity),
      m_slope_expand(expansion_slope),
      m_degres_expand(expansion_degressivity) {}

double Tractor_ShockForceRear::evaluate(double time,
                                        double rest_length,
                                        double length,
                                        double vel,
                                        const ChLinkTSDA& link) {
    // Simple model of a degressive damping characteristic

    double force = 0;

    // Calculate Damping Force
    if (vel >= 0) {
        force = -m_slope_expand / (1.0 + m_degres_expand * std::abs(vel)) * vel;
    } else {
        force = -m_slope_compr / (1.0 + m_degres_compr * std::abs(vel)) * vel;
    }

    return force;
}

Kraz_tractor_RearSuspension::Kraz_tractor_RearSuspension(const std::string& name) : ChLeafspringAxle(name) {
    m_springForceCB =
        chrono_types::make_shared<Tractor_SpringForceRear>(m_springCoefficient, m_springMinLength, m_springMaxLength);

    m_shockForceCB = chrono_types::make_shared<Tractor_ShockForceRear>(
        m_damperCoefficient, m_damperDegressivityCompression, m_damperCoefficient, m_damperDegressivityExpansion);
}

const ChVector3d Kraz_tractor_RearSuspension::getLocation(PointId which) {
    switch (which) {
        case SPRING_A:
            return ChVector3d(0.0, 0.429, m_axleTubeRadius);
        case SPRING_C:
            return ChVector3d(0.0, 0.429, m_axleTubeRadius + m_springDesignLength);
        case SHOCK_A:
            return ChVector3d(-0.15, 0.4075, m_axleTubeRadius - 0.05);
        case SHOCK_C:
            return ChVector3d(0.0, 0.329, m_axleTubeRadius + m_springDesignLength + 0.2);
        case SPINDLE:
            return ChVector3d(0.0, 1.00 - m_twin_tire_dist / 2.0, 0.0);
        default:
            return ChVector3d(0, 0, 0);
    }
}

}  // end namespace kraz
}  // end namespace vehicle
}  // end namespace chrono

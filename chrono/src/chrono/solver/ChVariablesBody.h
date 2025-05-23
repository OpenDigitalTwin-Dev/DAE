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
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#ifndef CHVARIABLESBODY_H
#define CHVARIABLESBODY_H

#include "chrono/core/ChMatrix33.h"
#include "chrono/solver/ChVariables.h"

namespace chrono {

/// Specialized class for representing a 6-DOF 3D rigid body, with mass matrix and associated variables.
/// This is an abstract class, specialized for example in ChVariablesBodyOwnMass and ChVariablesBodySharedMass.
class ChApi ChVariablesBody : public ChVariables {
  public:
    ChVariablesBody() : ChVariables(6), user_data(NULL) {}
    virtual ~ChVariablesBody() {}

    /// Assignment operator: copy from other object.
    ChVariablesBody& operator=(const ChVariablesBody& other);

    /// Get the mass associated with translation of body.
    virtual double GetBodyMass() const = 0;

    /// Access the 3x3 inertia matrix.
    virtual ChMatrix33<>& GetBodyInertia() = 0;
    virtual const ChMatrix33<>& GetBodyInertia() const = 0;

    /// Access the 3x3 inertia matrix inverted.
    virtual ChMatrix33<>& GetBodyInvInertia() = 0;
    virtual const ChMatrix33<>& GetBodyInvInertia() const = 0;

    void* GetUserData() { return this->user_data; }
    void SetUserData(void* mdata) { this->user_data = mdata; }

  private:
    void* user_data;
};

}  // end namespace chrono

#endif

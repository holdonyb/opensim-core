#ifndef MOCO_CASOCTRAPEZOIDAL_H
#define MOCO_CASOCTRAPEZOIDAL_H
/* -------------------------------------------------------------------------- *
 * OpenSim Moco: CasOCTrapezoidal.h                                           *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2018 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Christopher Dembia                                              *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0          *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "CasOCTranscription.h"

namespace CasOC {

/// Enforce the differential equations in the problem using a trapezoidal
/// (second-order) approximation. The integral in the objective function is
/// approximated by trapezoidal quadrature.
class Trapezoidal : public Transcription {
public:
    Trapezoidal(const Solver& solver, const Problem& problem)
        : Transcription(solver, problem, solver.getNumMeshPoints(), 
            solver.getNumMeshPoints()) { 

        OPENSIM_THROW_IF(problem.getEnforceConstraintDerivatives(),
            OpenSim::Exception, "Enforcing kinematic constraint derivatives "
            "not supported with trapezoidal transcription.");
        createVariablesAndSetBounds();
    }

private:
    casadi::DM createQuadratureCoefficientsImpl() const override;
    casadi::DM createKinematicConstraintIndicesImpl() const override;
    void applyConstraintsImpl(const VariablesMX& vars, const casadi::MX& xdot,
            const casadi::MX& residual, const casadi::MX& kcerr,
            const casadi::MXVector& path) override;

};

} // namespace CasOC

#endif // MOCO_CASOCTRAPEZOIDAL_H

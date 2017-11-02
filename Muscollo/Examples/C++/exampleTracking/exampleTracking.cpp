/* -------------------------------------------------------------------------- *
 * OpenSim Muscollo: exampleTracking.cpp                                      *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2017 Stanford University and the Authors                     *
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

/// This example solves a basic coordinate tracking problem using a double
/// pendulum.

#include <OpenSim/Simulation/SimbodyEngine/PinJoint.h>
#include <OpenSim/Actuators/CoordinateActuator.h>
#include <Muscollo/osimMuscollo.h>

using namespace OpenSim;

/// This model is torque-actuated.
Model createDoublePendulumModel() {
    Model model;
    model.setName("dp");

    using SimTK::Vec3;
    using SimTK::Inertia;

    // Create two links, each with a mass of 1 kg, center of mass at the body's
    // origin, and moments and products of inertia of zero.
    auto* b0 = new OpenSim::Body("b0", 1, Vec3(0), Inertia(1));
    model.addBody(b0);
    auto* b1  = new OpenSim::Body("b1", 1, Vec3(0), Inertia(1));
    model.addBody(b1);

    // Connect the bodies with pin joints. Assume each body is 1 m long.
    auto* j0 = new PinJoint("j0", model.getGround(), Vec3(0), Vec3(0),
            *b0, Vec3(-1, 0, 0), Vec3(0));
    auto& q0 = j0->updCoordinate();
    q0.setName("q0");
    auto* j1 = new PinJoint("j1",
            *b0, Vec3(0), Vec3(0), *b1, Vec3(-1, 0, 0), Vec3(0));
    auto& q1 = j1->updCoordinate();
    q1.setName("q1");
    model.addJoint(j0);
    model.addJoint(j1);

    auto* tau0 = new CoordinateActuator();
    tau0->setCoordinate(&j0->updCoordinate());
    tau0->setName("tau0");
    tau0->setOptimalForce(1);
    model.addComponent(tau0);

    auto* tau1 = new CoordinateActuator();
    tau1->setCoordinate(&j1->updCoordinate());
    tau1->setName("tau1");
    tau1->setOptimalForce(1);
    model.addComponent(tau1);

    return model;
}

// TODO create formal CoordinateTracking task.
class MucoCustomCoordinateTrackingCost : public MucoCost {
OpenSim_DECLARE_CONCRETE_OBJECT(MucoCustomCoordinateTrackingCost, MucoCost);
protected:
    void calcIntegralCostImpl(const SimTK::State& state,
            double& integrand) const override {
        // TODO more of a dancing or swaying movement.
        const auto& time = state.getTime();
        SimTK::Vector Qdesired(2);
        Qdesired[0] = time * 0.50 * SimTK::Pi;
        Qdesired[1] = time * 0.25 * SimTK::Pi;
        integrand = (state.getQ() - Qdesired).normSqr();
    }
};

int main() {

    MucoTool muco;
    muco.setName("double_pendulum_tracking");

    // Define the optimal control problem.
    // ===================================
    MucoProblem& mp = muco.updProblem();

    // Model (dynamics).
    // -----------------
    mp.setModel(createDoublePendulumModel());

    // Bounds.
    // -------
    mp.setTimeBounds(0, 1);
    mp.setStateInfo("j0/q0/value", {-10, 10});
    mp.setStateInfo("j0/q0/speed", {-50, 50});
    mp.setStateInfo("j1/q1/value", {-10, 10});
    mp.setStateInfo("j1/q1/speed", {-50, 50});
    mp.setControlInfo("tau0", {-100, 100});
    mp.setControlInfo("tau1", {-100, 100});

    // Cost.
    // -----
    MucoCustomCoordinateTrackingCost tracking;
    mp.addCost(tracking);

    // Configure the solver.
    // =====================
    MucoTropterSolver& ms = muco.initSolver();
    ms.set_num_mesh_points(50);
    ms.set_verbosity(2);
    ms.set_optim_solver("ipopt");
    ms.set_optim_hessian_approximation("exact");

    // Solve the problem.
    // ==================
    MucoSolution solution = muco.solve();
    solution.write("exampleTracking_solution.sto");

    muco.visualize(solution);

    return EXIT_SUCCESS;
}
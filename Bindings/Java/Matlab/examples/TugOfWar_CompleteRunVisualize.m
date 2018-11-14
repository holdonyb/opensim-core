%% TugOfWar_CompleteRunVisualize.m
%   Run a forward simulation with the tug of war model created
%   in by OpenSimCreateTugOfWarModel.m.

% ----------------------------------------------------------------------- %
% The OpenSim API is a toolkit for musculoskeletal modeling and           %
% simulation. See http://opensim.stanford.edu and the NOTICE file         %
% for more information. OpenSim is developed at Stanford University       %
% and supported by the US National Institutes of Health (U54 GM072970,    %
% R24 HD065690) and by DARPA through the Warrior Web program.             %
%                                                                         %
% Copyright (c) 2005-2019 Stanford University and the Authors             %
%                                                                         %
% Licensed under the Apache License, Version 2.0 (the "License");         %
% you may not use this file except in compliance with the License.        %
% You may obtain a copy of the License at                                 %
% http://www.apache.org/licenses/LICENSE-2.0.                             %
%                                                                         %
% Unless required by applicable law or agreed to in writing, software     %
% distributed under the License is distributed on an "AS IS" BASIS,       %
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or         %
% implied. See the License for the specific language governing            %
% permissions and limitations under the License.                          %
% ----------------------------------------------------------------------- %

%% Import OpenSim libraries
import org.opensim.modeling.*

%% Instantiate a model and setup the viualizer
% Instantiate TugOfWar model from file.
osimModel = Model('tug_of_war_muscles_controller.osim');
% Set up Simbody Visualizer to show the model and simulation.
osimModel.setUseVisualizer(true);
% Initializing the model.
osimModel.initSystem();

%% Define FwdTool, which will manage the simulation
% Define the new tool object which will be run.
tool = ForwardTool();
tool.setName('tugOfWar');
% Set the model on that the forward tool will operate on.
tool.setModel(osimModel);
% Set the start and end times of the simulaiton
tool.setStartTime(0);
tool.setFinalTime(3);
% Set the simulation to solve for euilibrium states of the muscle.
tool.setSolveForEquilibrium(true);
% Run the simulation
tool.run();

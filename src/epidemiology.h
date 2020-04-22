// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#ifndef EPIDEMIOLOGY_H_
#define EPIDEMIOLOGY_H_

#include "biodynamo.h"

#include "agent.h"
#include "behavior.h"
#include "evaluate.h"
#include "sim-param.h"

namespace bdm {

// This is the main simulation function
inline const std::string Simulate(CommandLineOptions* clo, TMultiGraph* mg) {
  // get command line option values
  auto visualize = clo->Get<bool>("visualize");
  auto moving_agents_ratio = clo->Get<double>("moving-agents-ratio");

  // Overwrite the parameters in the config file with the value from
  // the command line options
  auto set_param = [&](Param* param) {
    param->export_visualization_ = visualize;
    auto* sparam = param->GetModuleParam<SimParam>();
    sparam->moving_agents_ratio = moving_agents_ratio;
  };

  // Create simulation object
  Simulation sim(clo, set_param, "../config.toml");

  // Get pointers to important objects
  auto* param = sim.GetParam();
  auto* sparam = param->GetModuleParam<SimParam>();
  auto* random = sim.GetRandom();
  ResultData result;

  // initialize random number generators
  for (int i = 1; i < ThreadInfo::GetInstance()->GetMaxThreads(); i++) {
    sim.GetAllRandom()[i]->SetSeed(4357 * i);
  }

  auto state = State::kSusceptible;
  // Lambda that creates a new agent at specific position in space
  auto agent_creator = [&](const Double3& position) {
    auto* agent = new Agent(position);
    // Set the data members of the new agent
    agent->SetDiameter(sparam->agent_diameter);
    agent->state_ = state;
    agent->recovery_counter_ = sparam->recovery_duration;
    // Define the agents behavior
    agent->AddBiologyModule(new CatchVirus());
    agent->AddBiologyModule(new Recover());
    if (random->Uniform() < sparam->moving_agents_ratio) {
      agent->AddBiologyModule(new MoveRandomly());
    }
    return agent;
  };

  // Create an initial population of susceptible agents.
  // Agents will be randomly distributed.
  ModelInitializer::CreateCellsRandom(param->min_bound_, param->max_bound_,
                                      sparam->initial_population_susceptible,
                                      agent_creator);

  // Create an initial population of infected agents.
  // Agents will be randomly distributed.
  state = State::kInfected;
  ModelInitializer::CreateCellsRandom(param->min_bound_, param->max_bound_,
                                      sparam->initial_population_infected,
                                      agent_creator);

  // workaround for bug -> ignore
  auto* cell = new Cell();
  cell->SetDiameter(sparam->infection_radius);
  sim.GetResourceManager()->push_back(cell);
  // end of workaround

  // Now we finished defining the initial simulation state.

  // Simulate for SimParam::number_of_iterations steps
  for (uint64_t i = 0; i < sparam->number_of_iterations; ++i) {
    sim.GetScheduler()->Simulate(1);
    // Count how many agents are infected.
    UpdateResult(&sim, &result, i);
  }

  bdm::PlotResults(result, mg);
  return Concat(sim.GetOutputDir(), "/", sparam->result_plot);
}

}  // namespace bdm

#endif  // EPIDEMIOLOGY_H_

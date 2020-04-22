// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#include "sim-param.h"
#include "core/param/param.h"
#include "core/util/cpptoml.h"

#define BDM_ASSIGN_PARAM_VALUE(value) BDM_ASSIGN_CONFIG_VALUE(value, #value)

namespace bdm {

const ModuleParamUid SimParam::kUid = ModuleParamUidGenerator::Get()->NewUid();

ModuleParam* SimParam::GetCopy() const { return new SimParam(*this); }

ModuleParamUid SimParam::GetUid() const { return kUid; }

void SimParam::AssignFromConfig(const std::shared_ptr<cpptoml::table>& config) {
  BDM_ASSIGN_PARAM_VALUE(result_plot);
  BDM_ASSIGN_PARAM_VALUE(number_of_iterations);
  BDM_ASSIGN_PARAM_VALUE(initial_population_susceptible);
  BDM_ASSIGN_PARAM_VALUE(initial_population_infected);
  BDM_ASSIGN_PARAM_VALUE(infection_radius);
  BDM_ASSIGN_PARAM_VALUE(moving_agents_ratio);
  BDM_ASSIGN_PARAM_VALUE(recovery_duration);
  BDM_ASSIGN_PARAM_VALUE(agent_diameter);
  BDM_ASSIGN_PARAM_VALUE(agent_speed);
  BDM_ASSIGN_PARAM_VALUE(infection_probablity);
}

}  // namespace bdm

// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#ifndef SIM_PARAM_H_
#define SIM_PARAM_H_

#include "core/param/module_param.h"

namespace bdm {

/// This class defines parameters that are specific to this simulation.
struct SimParam : public ModuleParam {
  static const ModuleParamUid kUid;

  ModuleParam* GetCopy() const override;

  ModuleParamUid GetUid() const override;

  SimParam() {}

  std::string result_plot = "result";
  uint64_t number_of_iterations = 300;
  uint64_t initial_population_susceptible = 1000;
  uint64_t initial_population_infected = 1;
  double infection_radius = 10;
  double moving_agents_ratio = 1.0;
  double recovery_duration = 30;
  double agent_diameter = 1.0;
  double agent_speed = 1;
  double infection_probablity = 0.1;

 protected:
  /// Assign values from config file to variables
  void AssignFromConfig(const std::shared_ptr<cpptoml::table>&) override;

 private:
  BDM_CLASS_DEF_OVERRIDE(SimParam, 1);
};

}  // namespace bdm

#endif  // SIM_PARAM_H_

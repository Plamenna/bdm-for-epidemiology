// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "core/biology_module/biology_module.h"

#include "agent.h"
#include "sim-param.h"

namespace bdm {

// -----------------------------------------------------------------------------
struct CheckSurrounding : public Functor<void, const SimObject*, double> {
  Agent* self_;
  double recovery_duration_;

  CheckSurrounding(Agent* self, double recovery_duration)
      : self_(self), recovery_duration_(recovery_duration) {}

  // This function operator will be called for every other agent within
  // `infection_radius`
  void operator()(const SimObject* neighbor, double squared_distance) override {
    auto* other = bdm_static_cast<const Agent*>(neighbor);
    if (other->state_ == State::kInfected) {
      self_->state_ = State::kInfected;
      self_->recovery_counter_ = recovery_duration_;
    }
  }
};

// -----------------------------------------------------------------------------
struct CatchVirus : public BaseBiologyModule {
  BDM_STATELESS_BM_HEADER(CatchVirus, BaseBiologyModule, 1);

  CatchVirus() : BaseBiologyModule(gAllEventIds) {}

  void Run(SimObject* so) override {
    auto* sim = Simulation::GetActive();
    auto* param = sim->GetParam();
    auto* sparam = param->GetModuleParam<SimParam>();
    auto* ctxt = sim->GetExecutionContext();

    auto* agent = bdm_static_cast<Agent*>(so);
    if (agent->state_ == kSusceptible) {
      CheckSurrounding check(agent, sparam->recovery_duration);
      ctxt->ForEachNeighborWithinRadius(check, *agent,
                                        sparam->infection_radius);
    }
  }
};

// -----------------------------------------------------------------------------
struct Recover : public BaseBiologyModule {
  BDM_STATELESS_BM_HEADER(Recover, BaseBiologyModule, 1);

  Recover() : BaseBiologyModule(gAllEventIds) {}

  void Run(SimObject* so) override {
    auto* agent = bdm_static_cast<Agent*>(so);
    if (agent->state_ == kInfected) {
      if (agent->recovery_counter_ <= 0) {
        agent->state_ = State::kRecovered;
      } else {
        agent->recovery_counter_--;
      }
    }
  }
};

// -----------------------------------------------------------------------------
struct MoveRandomly : public BaseBiologyModule {
  BDM_STATELESS_BM_HEADER(MoveRandomly, BaseBiologyModule, 1);

  MoveRandomly() : BaseBiologyModule(gAllEventIds) {}

  void Run(SimObject* so) override {
    auto* sim = Simulation::GetActive();
    auto* random = sim->GetRandom();
    auto* param = sim->GetParam();
    auto* sparam = param->GetModuleParam<SimParam>();

    auto* agent = bdm_static_cast<Agent*>(so);

    const auto& position = agent->GetPosition();
    auto rand_movement = random->UniformArray<3>(-1, 1).Normalize();
    agent->SetPosition(position + rand_movement * sparam->agent_speed);
  }
};

}  // namespace bdm

#endif  // BEHAVIOR_H_

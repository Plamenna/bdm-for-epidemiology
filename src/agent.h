// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#ifndef AGENT_H_
#define AGENT_H_

#include "core/sim_object/cell.h"

namespace bdm {

/// Possible agent states.
enum State { kSusceptible, kInfected, kRecovered };

class Agent : public Cell {
  BDM_SIM_OBJECT_HEADER(Agent, Cell, 1, state_, recovery_counter_);

 public:
  Agent() {}
  Agent(const Event& event, SimObject* other, uint64_t new_oid = 0)
      : Base(event, other, new_oid) {}
  explicit Agent(const Double3& position) : Base(position) {}

  /// This data member stores the current state of the agent.
  int state_ = State::kSusceptible;
  /// Once the agent becomes infected, this counter is set to the
  /// recovery duration.
  double recovery_counter_ = 0;
};

}  // namespace bdm

#endif  // AGENT_H_

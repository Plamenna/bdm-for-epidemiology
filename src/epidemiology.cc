// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#include "epidemiology.h"
#include "evaluate.h"
#include "sim-param.h"

int main(int argc, const char** argv) {
  // define addtional command line options
  bdm::CommandLineOptions clo(argc, argv);
  clo.AddOption<bool>("visualize", "false");
  clo.AddOption<double>("moving-agents-ratio", "1.0");

  // register parameters that are specific for this simulation
  bdm::Param::RegisterModuleParam(new bdm::SimParam());

  // setup plot
  TCanvas c;
  TMultiGraph mg;
  bdm::InitPlot(&c, &mg);
  auto plot_filename = bdm::Simulate(&clo, &mg);
  bdm::FinalizePlot(&c, &mg, plot_filename.c_str());

  return 0;
}

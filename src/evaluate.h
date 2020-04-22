// -----------------------------------------------------------------------------
//
// Copyright (C) Lukas Breitwieser.
// All Rights Reserved.
//
// -----------------------------------------------------------------------------

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <cmath>
#include <vector>

#include <TAxis.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TPad.h>

#include "core/resource_manager.h"
#include "core/simulation.h"

namespace bdm {

// ---------------------------------------------------------------------------
struct ResultData {
  std::vector<double> time_;
  std::vector<double> infected_;
};

// ---------------------------------------------------------------------------
inline void UpdateResult(Simulation* sim, ResultData* result, double time) {
  auto* rm = sim->GetResourceManager();
  double infected = 0;
  rm->ApplyOnAllElements([&](SimObject* so) {
    auto* agent = bdm_static_cast<Agent*>(so);
    infected += agent->state_ == State::kInfected;
  });
  result->time_.push_back(time);
  // -1 because an additional cell has been added as a workaround
  auto num_agents = rm->GetNumSimObjects() - 1;
  result->infected_.push_back(infected / num_agents);
}

// ---------------------------------------------------------------------------
inline void InitPlot(TCanvas* c, TMultiGraph* mg) {
  c->SetGrid();
  c->SetRightMargin(0.05);
  c->SetTopMargin(0.05);

  mg->SetTitle("Infected Agents;Iterations;Infected/Total Population");
}

// ---------------------------------------------------------------------------
inline void PlotResults(const ResultData& result, TMultiGraph* mg) {
  uint64_t n = result.time_.size();

  auto* gr = new TGraph(n, result.time_.data(), result.infected_.data());
  gr->SetTitle("Infected Agents");
  gr->SetFillStyle(0);
  gr->SetLineWidth(2);
  mg->SetMaximum(1.);

  mg->Add(gr, "L");
}

// ---------------------------------------------------------------------------
inline void FinalizePlot(TCanvas* c, TMultiGraph* mg, const char* filename) {
  mg->Draw("A");

  // finalize plot
  // TCanvas::Update() draws the frame, after which one can change it
  c->Update();
  c->GetFrame()->SetBorderSize(12);
  gPad->Modified();
  gPad->Update();
  c->Modified();
  c->cd(0);
  c->SaveAs(Concat(std::string(filename), ".svg").c_str());
}

// ---------------------------------------------------------------------------
inline void ResultDataToCsv(const ResultData& result,
                            const std::string& filename) {
  std::ofstream str(filename);
  str << "Time,Infected\n";
  for (uint64_t i = 0; i < result.infected_.size(); i++) {
    str << result.time_[i] << "," << result.infected_[i] << "\n";
  }
  str.close();
}

}  // namespace bdm

#endif  // EVALUATE_H_

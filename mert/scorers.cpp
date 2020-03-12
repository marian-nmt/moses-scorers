#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "Scorer.h"
#include "ScorerFactory.h"
#include "Types.h"

using namespace MosesTuning;

// float score(std::string scorerType, stdstd::vector<std::string> refFiles,
// std::vector<std::string> cands, std::string scorerConfig = "", std::string postproc = "") {

int main(int argc, char **argv) {
  if(argc == 1) {
    std::cerr << "Usage: ./run-scorer sctype ref1 < candidate > scores" << std::endl;
    return 1;
  }

  std::string sctype = argv[1];
  std::vector<std::string> refFiles(argv + 2, argv + argc);

  boost::scoped_ptr<Scorer> scorer(ScorerFactory::getScorer(sctype, ""));
  scorer->setReferenceFiles(refFiles);

  std::string hyp;
  ScoreStats scoreStats;
  size_t sid = 0;

  while(getline(std::cin, hyp)) {
    scorer->prepareStats(sid, hyp, scoreStats);
    ++sid;
  }

  std::vector<float> stats(scoreStats.getArray(), scoreStats.getArray() + scoreStats.size());
  std::cout << scorer->calculateScore(stats) << std::endl;

  return 0;
}

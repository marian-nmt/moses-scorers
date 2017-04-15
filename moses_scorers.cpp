#include "moses_scorers.h"

#include "Scorer.h"
#include "ScorerFactory.h"
#include "Types.h"

#include <fstream>
#include <memory>

float score(const std::string& scorerType,
            const std::vector<std::string>& refFiles,
            std::istream& candidate,
            const std::string& scorerConfig,
            const std::string& preproc) {

  std::unique_ptr<MosesTuning::Scorer> scorer(
      MosesTuning::ScorerFactory::getScorer(scorerType, scorerConfig));
  scorer->setFilter(preproc);
  scorer->setReferenceFiles(refFiles);

  std::string hyp;
  MosesTuning::ScoreStats scoreStats;
  size_t sid = 0;

  while (getline(candidate, hyp)) {
    scorer->prepareStats(sid, hyp, scoreStats);
    ++sid;
  }

  std::vector<float> stats(scoreStats.getArray(),
                           scoreStats.getArray() + scoreStats.size());
  return scorer->calculateScore(stats);
}

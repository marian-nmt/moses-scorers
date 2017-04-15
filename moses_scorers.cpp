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
  MosesTuning::ScoreStats sentStats;
  std::vector<float> corpStats;

  size_t sid = 0;
  while (getline(candidate, hyp)) {
    scorer->prepareStats(sid, hyp, sentStats);

    if (corpStats.size() == 0) {
      corpStats = std::vector<float>(sentStats.getArray(), sentStats.getArray() + sentStats.size());
    } else {
      for (size_t i = 0; i < sentStats.size(); ++i) {
        corpStats[i] += sentStats.get(i);
      }
    }

    ++sid;
  }

  return scorer->calculateScore(corpStats);
}

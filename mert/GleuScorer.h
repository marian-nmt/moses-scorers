#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "Ngram.h"
#include "Reference.h"
#include "ScopedVector.h"
#include "ScoreData.h"
#include "StatisticsBasedScorer.h"
#include "Types.h"

namespace MosesTuning {

const std::string ToLower(const std::string& str);
size_t NumberOfNgrams(const NgramCounts& counts, size_t n);

/**
 * Gleu scoring
 */
class GleuScorer : public StatisticsBasedScorer {
public:
  explicit GleuScorer(const std::string& config = "");
  ~GleuScorer();

  virtual void setReferenceFiles(const std::vector<std::string>& referenceFiles);
  virtual void prepareStats(std::size_t sid, const std::string& text, ScoreStats& entry);
  virtual statscore_t calculateScore(const std::vector<ScoreStatsType>& comps) const;

  virtual std::size_t NumberOfScores() const { return NumberOfReferences() * (2 * m_order + 1); }
  virtual std::size_t NumberOfReferences() const {
    // TODO: an ugly hack for knowing the number of multiple references before reading reference
    // files
    return m_numrefs;
  }

  virtual float getReferenceLength(const std::vector<ScoreStatsType>& totals) const {
    // TODO: take into account all references, not just the first one
    return totals[m_order * 2];
  }

  void CalcGleuStats(const std::string& hypText,
                     const std::vector<NgramCounts>& counts,
                     ScoreStats& entry) const;
  float calculateGleu(const std::vector<ScoreStatsType>& stats, bool smooth = false) const;

  const std::vector<NgramCounts>& GetReference(size_t sid) const {
    UTIL_THROW_IF2(sid >= m_references.size(),
                   "Sentence id (" << sid << ") not found in reference set.");
    return *(m_references.get())[sid];
  }

private:
  size_t CountNgrams(const std::string& line,
                     NgramCounts& counts,
                     unsigned int n,
                     bool is_testing = false) const;
  void CountDiffNgrams(const NgramCounts& countsA,
                       const NgramCounts& countsB,
                       NgramCounts& resultCounts) const;

  std::vector<ScoreStatsType> CalcGleuStatsForSingleRef(const NgramCounts& hypCounts,
                                                        const NgramCounts& srcCounts,
                                                        const NgramCounts& refCounts) const;
  float calculateGleuForSingleRef(const std::vector<ScoreStatsType>& comps,
                                  bool smooth = false) const;

  // maximum order of ngrams
  size_t m_order;
  // show debug messages
  bool m_debug;
  // neglect original casing
  bool m_lowercase;
  // apply smoothing
  bool m_smooth;

  // number of references
  size_t m_numrefs;

  // source and multiple reference sentences
  ScopedVector<std::vector<NgramCounts>> m_references;

  // no copying allowed
  GleuScorer(const GleuScorer&);
  GleuScorer& operator=(const GleuScorer&);
};

}  // namespace MosesTuning

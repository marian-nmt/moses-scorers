#pragma once

#include <string>
#include <vector>
#include <iostream>

/**
 * @brief Score candidate sentences with a chosen Moses scorer.
 *
 * @param scorerType Scorer name.
 * @param refFiles List of paths to reference files.
 * @param candidate Input stream of sentences to be scored.
 * @param scorerConfig Configuration option to the scorer in the form of
 *   "key1:val1,key2:val2".
 * @param preproc Unix command, which will be used to preprocess the sentences
 *   before scoring.
 *
 * @return Score.
 */
extern float score(const std::string& scorerType,
                   const std::vector<std::string>& refFiles,
                   std::istream& candidate,
                   const std::string& scorerConfig = "",
                   const std::string& preproc = "");

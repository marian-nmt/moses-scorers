#include <getopt.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "Util.h"
#include "TER/tercalc.h"

void usage() {
  using namespace std;

  cerr << "usage: paste cand ref | teralign [options]" << endl;
  cerr << "[--invert|-i] report inverted 1 - TER score" << endl;
  cerr << "[--clamp|-c] clamp scores to 0-1 range" << endl;
  cerr << "[--tokenize|-t] tokenize inputs sacrebleu-style" << endl;
  cerr << "[--print|-p] print (tokenized) inputs" << endl;
  cerr << "[--alignment|-a] print full alignment" << endl;
  cerr << "[--wmt|-w] output OK/BAD tags a la WMT QE task (1-to-1 with cand tokens)" << endl;
  cerr << "[--help|-h] print this message and exit" << endl;
  cerr << endl;
  exit(1);
}

static struct option long_options[] = {{"invert", no_argument, 0, 'i'},
                                       {"clamp", no_argument, 0, 'c'},
                                       {"tokenize", no_argument, 0, 't'},
                                       {"print", no_argument, 0, 'p'},
                                       {"alignment", no_argument, 0, 'a'},
                                       {"wmt", no_argument, 0, 'w'},
                                       {"help", no_argument, 0, 'h'},
                                       {0, 0, 0, 0}};

// Options used in evaluator.
struct ProgramOption {
  bool printAlignment{false};
  bool printAlignmentWMT{false};
  bool negateScore{false};
  bool clampScore{false};
  bool tokenize{false};
  bool print{false};
  
  ProgramOption() {}
};

void ParseCommandOptions(int argc, char** argv, ProgramOption* opt) {
  int c;
  int option_index;
  while((c = getopt_long(argc, argv, "awicthp", long_options, &option_index)) != -1) {
    switch(c) {
      case 'a': opt->printAlignment = true; break;
      case 'w': opt->printAlignmentWMT = true; break;
      case 'i': opt->negateScore = true; break;
      case 'c': opt->clampScore = true; break;
      case 't': opt->tokenize = true; break;
      case 'p': opt->print = true; break;
      default: usage();
    }
  }
}

namespace mt = MosesTuning;
namespace TER = TERCPPNS_TERCpp;

static std::string tokenize(const std::string& text) {
  std::string normText = text;
  namespace regex = std;

  // language-independent part:
  normText = regex::regex_replace(normText, regex::regex("<skipped>"), ""); // strip "skipped" tags
  normText = regex::regex_replace(normText, regex::regex("-\\n"), "");      // strip end-of-line hyphenation and join lines
  normText = regex::regex_replace(normText, regex::regex("\\n"), " ");      // join lines
  normText = regex::regex_replace(normText, regex::regex("&quot;"), "\"");  // convert SGML tag for quote to "
  normText = regex::regex_replace(normText, regex::regex("&amp;"), "&");    // convert SGML tag for ampersand to &
  normText = regex::regex_replace(normText, regex::regex("&lt;"), "<");     //convert SGML tag for less-than to >
  normText = regex::regex_replace(normText, regex::regex("&gt;"), ">");     //convert SGML tag for greater-than to <

  // language-dependent part (assuming Western languages):
  normText = " " + normText + " ";
  normText = regex::regex_replace(normText, regex::regex("([\\{-\\~\\[-\\` -\\&\\(-\\+\\:-\\@\\/])"), " $1 "); // tokenize punctuation
  normText = regex::regex_replace(normText, regex::regex("([^0-9])([\\.,])"), "$1 $2 "); // tokenize period and comma unless preceded by a digit
  normText = regex::regex_replace(normText, regex::regex("([\\.,])([^0-9])"), " $1 $2"); // tokenize period and comma unless followed by a digit
  normText = regex::regex_replace(normText, regex::regex("([0-9])(-)"), "$1 $2 ");       // tokenize dash when preceded by a digit
  normText = regex::regex_replace(normText, regex::regex("\\s+"), " "); // one space only between words
  normText = regex::regex_replace(normText, regex::regex("^\\s+"), ""); // no leading space
  normText = regex::regex_replace(normText, regex::regex("\\s+$"), ""); // no trailing space

  return normText;
}

static inline std::string alignment2tags(const TER::terAlignment& aln) {
  std::stringstream tags;
  bool first = true;
  for(auto action : aln.alignment) {
    std::string delim = first ? "" : " ";
    switch (action) {
      case 'A': tags << delim << "OK";  first = false; break;
      case 'S': tags << delim << "BAD"; first = false; break;
      case 'I': tags << delim << "BAD"; first = false; break;
      case 'D': break; // do nothing
      default: break;
    }
  }
  return tags.str();
}

int main(int argc, char** argv) {
  ProgramOption option;
  ParseCommandOptions(argc, argv, &option);

  try {
    std::string line;
    TER::terCalc ter;
    while(std::getline(std::cin, line)) {
      std::vector<std::string> fields;
      mt::split(line, '\t', fields);
      std::vector<std::string> hyp, ref;

      if(option.tokenize) {
        fields[0] = tokenize(fields[0]);
        fields[1] = tokenize(fields[1]);
      }

      mt::split(fields[0], ' ', hyp);
      mt::split(fields[1], ' ', ref);

      TER::terAlignment result = ter.TER(hyp, ref);

      double score = result.score();
      if(option.negateScore)
        score = 1.0 - score;
      if(option.clampScore)
        score = std::max(0.0, std::min(1.0, score));

      std::cout << std::fixed << std::setprecision(4);
      std::cout << score;
      
      if(option.printAlignment || option.printAlignmentWMT) {
        if(option.printAlignmentWMT)
          std::cout << "\t" << alignment2tags(result);
        else
          std::cout << "\t" << result.printAlignments();
      }

      if(option.print) {
        std::cout << "\t" << fields[0];
        std::cout << "\t" << fields[1];
      }

      std::cout << std::endl;
    }
    
    return EXIT_SUCCESS;
  } catch(const std::exception& e) {
    std::cerr << "Exception: " << e.what() << endl;
    return EXIT_FAILURE;
  }
}

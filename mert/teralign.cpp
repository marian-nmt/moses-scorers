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
  cerr << "[--ok-bad|-w] output OK/BAD tags a la WMT QE task (1-to-1 with cand tokens)" << endl;
  cerr << "[--gaps|-g] add WMT18-style gap tokens" << endl;
  cerr << "[--beam-width|-b] beam width, optional, default is 20" << endl;
  cerr << "[--max-shift-distance|-d] maximum shift distance, optional, default is 50 tokens" << endl;
  cerr << "[--match-cost|-M] cost for matching, default is 0" << endl;
  cerr << "[--delete-cost|-D] cost for deleting, default is 1" << endl;
  cerr << "[--substitute-cost|-B] cost for substituting, default is 1" << endl;
  cerr << "[--insert-cost|-I] cost for inserting, default is 1" << endl;
  cerr << "[--shift-cost|-T] cost for shifting, default is 1" << endl;
  cerr << "[--wmt17|-7] print WMT17 style OK/BAD tags with no shifting and beam-width 20" << endl;
  cerr << "[--wmt18|-8] print WMT18 style OK/BAD tags with no shifting, beam-width 20 and gap tags" << endl;
  cerr << "[--help|-h] print this message and exit" << endl;
  cerr << endl;
  exit(1);
}

//  java -jar tercom.jar [-N] [-s] [-P] -r ref -h hyp [-a alter_ref] [-b beam_width]
// [-S trans_span_prefix] [-o out_format -n out_pefix] [-d max_shift_distance] 
// [-M match_cost] [-D delete_cost] [-B substitute_cost] [-I insert_cost] [-T shift_cost]

static struct option long_options[] = {{"invert", no_argument, 0, 'i'},
                                       {"clamp", no_argument, 0, 'c'},
                                       {"tokenize", no_argument, 0, 't'},
                                       {"print", no_argument, 0, 'p'},
                                       {"alignment", no_argument, 0, 'a'},
                                       {"ok-bad", no_argument, 0, 'w'},
                                       {"gaps", no_argument, 0, 'g'},
                                       {"beam-width", required_argument, 0, 'b'},
                                       {"max-shift-distance", required_argument, 0, 'd'},
                                       {"match-cost", required_argument, 0, 'M'},
                                       {"delete-cost", required_argument, 0, 'D'},
                                       {"substitute-cost", required_argument, 0, 'B'},
                                       {"insert-cost", required_argument, 0, 'I'},
                                       {"shift-cost", required_argument, 0, 'T'},
                                       {"wmt17", no_argument, 0, '7'},
                                       {"wmt18", no_argument, 0, '8'},
                                       {"help", no_argument, 0, 'h'},
                                       {0, 0, 0, 0}};

// Options used in evaluator.
struct ProgramOption {
  bool printAlignment{false};
  bool printOkBad{false};
  bool negateScore{false};
  bool clampScore{false};
  bool tokenize{false};
  bool print{false};
  bool gaps{false};

  int beamWidth{20};
  int maxShiftDistance{50};
  int matchCost{0};
  int deleteCost{1};
  int substituteCost{1};
  int insertCost{1};
  int shiftCost{1};

  ProgramOption() {}
};

void ParseCommandOptions(int argc, char** argv, ProgramOption* opt) {
  int c;
  int option_index;
  while((c = getopt_long(argc, argv, "awgicthp78b:d:M:D:B:I:T:", long_options, &option_index)) != -1) {
    switch(c) {
      case 'a': opt->printAlignment = true; break;
      case 'w': opt->printOkBad = true; break;
      case 'g': opt->gaps = true; break;
      case 'i': opt->negateScore = true; break;
      case 'c': opt->clampScore = true; break;
      case 't': opt->tokenize = true; break;
      case 'p': opt->print = true; break;
      case 'b': opt->beamWidth = std::atoi(optarg); break;
      case 'd': opt->maxShiftDistance = std::atoi(optarg); break;
      case 'M': opt->matchCost = std::atoi(optarg); break;
      case 'D': opt->deleteCost = std::atoi(optarg); break;
      case 'B': opt->substituteCost = std::atoi(optarg); break;
      case 'I': opt->insertCost = std::atoi(optarg); break;
      case 'T': opt->shiftCost = std::atoi(optarg); break;

      case '7': 
        opt->printOkBad = true; 
        opt->maxShiftDistance = 0;
        opt->beamWidth = 20;
        break;

      case '8': 
        opt->printOkBad = true; 
        opt->maxShiftDistance = 0;
        opt->beamWidth = 20;
        opt->gaps = true;
        break;

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

static inline std::string alignment2tags(const TER::terAlignment& aln, bool hasGaps) {
  std::stringstream tags;
  bool first = true;
  bool lastDel = false;
  for(auto action : aln.alignment) {
    std::string delim = first ? "" : " ";
    std::string gap = hasGaps ? (lastDel ? "BAD " : "OK ") : "";
    switch (action) {
      case 'A': tags << delim << gap << "OK";  first = false; lastDel = false; break;
      case 'S': tags << delim << gap << "BAD"; first = false; lastDel = false; break;
      case 'I': tags << delim << gap << "BAD"; first = false; lastDel = false; break;
      case 'D': lastDel = true; break; // do nothing
      default: break;
    }
  }
  if(hasGaps)
    tags << " " << (lastDel ? "BAD" : "OK");
  return tags.str();
}

int main(int argc, char** argv) {
  ProgramOption option;
  ParseCommandOptions(argc, argv, &option);

  try {
    std::string line;
    TER::terCalc ter(option.beamWidth,
                     option.maxShiftDistance, option.matchCost, option.deleteCost, 
                     option.substituteCost, option.insertCost, option.shiftCost);

    while(std::getline(std::cin, line)) {
      std::vector<std::string> fields;
      mt::split(line, '\t', fields);
      std::vector<std::string> hyp, ref;

      if(fields.size() != 2)
        throw std::runtime_error("Input seems to have too few or to many tab-separated fields");

      if(option.tokenize) {
        fields[0] = tokenize(fields[0]);
        fields[1] = tokenize(fields[1]);
      }

      mt::split(fields[0], ' ', hyp);
      mt::split(fields[1], ' ', ref);

      if(hyp.empty())
        throw std::runtime_error("Hypothesis is empty");
      
      if(ref.empty())
        throw std::runtime_error("Reference is empty");

      TER::terAlignment result = ter.TER(hyp, ref);

      double score = result.score();
      if(option.negateScore)
        score = 1.0 - score;
      if(option.clampScore)
        score = std::max(0.0, std::min(1.0, score));

      std::cout << std::fixed << std::setprecision(4);
      std::cout << score;
      
      if(option.printAlignment || option.printOkBad) {
        if(option.printOkBad)
          std::cout << "\t" << alignment2tags(result, option.gaps);
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

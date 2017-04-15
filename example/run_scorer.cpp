#include <iostream>
#include "moses_scorers.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Usage: ./run-scorer sctype scconfig ref1 [ref2...] < candidate > score" << std::endl;
    return 1;
  }

  std::string sctype = argv[1];
  std::string scconfig = argv[2];
  std::vector<std::string> refFiles(argv + 3, argv + argc);

  std::cout << score(sctype, refFiles, std::cin, scconfig) << std::endl;

  return 0;
}

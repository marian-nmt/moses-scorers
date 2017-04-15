#include <iostream>
#include "moses_scorers.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "Usage: ./run-scorer sctype ref1 < candidate > scores" << std::endl;
    return 1;
  }

  std::string sctype = argv[1];
  std::vector<std::string> refFiles(argv + 2, argv + argc);

  std::cout << score(sctype, refFiles, std::cin) << std::endl;

  return 0;
}

#include "util/random.hh"

#include <cstdlib>
#include <ctime>

namespace util
{

void rand_init(unsigned int seed)
{
  srand(seed);
}


void rand_init()
{
  rand_init(time(NULL));
}

namespace internal
{
// This is the one call to the actual randomizer.  All else is built on this.
int rand_int()
{
  return std::rand();
}
} // namespace internal
} // namespace util

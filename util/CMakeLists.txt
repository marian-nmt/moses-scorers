set(UTIL_SOURCES
  double-conversion/bignum.cc
  double-conversion/cached-powers.cc
  double-conversion/double-conversion.cc
  double-conversion/fixed-dtoa.cc
  double-conversion/bignum-dtoa.cc
  double-conversion/diy-fp.cc
  double-conversion/fast-dtoa.cc
  double-conversion/strtod.cc
  exception.cc
  float_to_string.cc
  random.cc
  integer_to_string.cc
  file_piece.cc
  murmur_hash.cc
  string_piece.cc
)

add_library(util OBJECT ${UTIL_SOURCES})

if(ENABLE_TESTS)
    add_library(utillib SHARED ${UTIL_SOURCES})
endif(ENABLE_TESTS)

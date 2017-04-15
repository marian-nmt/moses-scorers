add_library(util OBJECT
  double-conversion/bignum.cc
  double-conversion/cached-powers.cc
  double-conversion/double-conversion.cc
  double-conversion/fixed-dtoa.cc
  double-conversion/bignum-dtoa.cc
  double-conversion/diy-fp.cc
  double-conversion/fast-dtoa.cc
  double-conversion/strtod.cc

  bit_packing.cc
  exception.cc
  float_to_string.cc
  mmap.cc
  random.cc
  scoped.cc
  usage.cc
  file.cc
  integer_to_string.cc
  pool.cc
  file_piece.cc
  murmur_hash.cc
  read_compressed.cc
  ersatz_progress.cc
  parallel_read.cc
  string_piece.cc
)

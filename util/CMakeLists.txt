set(UTIL_SOURCES
  exception.cc
  file.cc
  random.cc
  murmur_hash.cc
  scoped.cc
  string_piece.cc
)

add_library(util OBJECT ${UTIL_SOURCES})

if(ENABLE_TESTS)
    add_library(utillib SHARED ${UTIL_SOURCES})
endif(ENABLE_TESTS)

set(MERT_SOURCES
    Util.cpp
    GzFileBuf.cpp
    FileStream.cpp
    Timer.cpp
    ScoreStats.cpp
    ScoreArray.cpp
    BleuScorer.cpp
    BleuDocScorer.cpp
    SemposScorer.cpp
    SemposOverlapping.cpp
    InterpolatedScorer.cpp
    PerScorer.cpp
    HwcmScorer.cpp
    InternalTree.cpp
    M2.cpp
    M2Scorer.cpp
    ScoreData.cpp
    Scorer.cpp
    ScorerFactory.cpp
    TER/alignmentStruct.cpp
    TER/hashMap.cpp
    TER/hashMapStringInfos.cpp
    TER/stringHasher.cpp
    TER/terAlignment.cpp
    TER/terShift.cpp
    TER/hashMapInfos.cpp
    TER/infosHasher.cpp
    TER/stringInfosHasher.cpp
    TER/tercalc.cpp
    TER/tools.cpp
    TER/bestShiftStruct.cpp
    TerScorer.cpp
    CderScorer.cpp
    MeteorScorer.cpp
    Vocabulary.cpp
    PreProcessFilter.cpp
    SentenceLevelScorer.cpp
    Permutation.cpp
    PermutationScorer.cpp
    StatisticsBasedScorer.cpp
    GleuScorer.cpp
)

add_library(mert OBJECT ${MERT_SOURCES})

if(ENABLE_TESTS)
    add_library(mertlib SHARED ${MERT_SOURCES})
    target_link_libraries(mertlib utillib)
endif(ENABLE_TESTS)

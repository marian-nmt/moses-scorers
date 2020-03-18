moses-scorers
=============

Scorers from Moses (https://github.com/moses-smt/moses-decoder) as a separate library.

Compilation without unit testing:

    mkdir build
    cd build
    cmake ..
    make -j

then example usage via 

    ./evaluator -R reference -C cand

and for sentence-level TER (with optional alignments)

    paste cand reference | ./teralign -ic --wmt17

which results in WMT-QE-task-style output (with 1-TER, clamped to 0-1 range):

```
0.0000  BAD BAD BAD
0.7778  OK OK OK OK OK OK OK BAD
0.8571  OK OK OK OK BAD OK OK
```

Using `--wmt18` will also add the gap symbols introduced for WMT18. We are working on adding the source word tags:

    paste cand reference | ./teralign -ic --wmt18

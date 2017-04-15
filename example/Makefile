all: test

test: run-scorer
	./run-scorer BLEU case:1 newstest2015.ende.de < newstest2015.single.out

run-scorer: run_scorer.cpp
	$(CXX) -std=c++11 -o $@ $? -I.. -L../build/ -lmosesscorers -lz -lboost_filesystem -lboost_system

clean:
	rm -f run-scorer

.PHONY: clean test

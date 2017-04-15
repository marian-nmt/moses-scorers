all: test

test: run-scorer
	./run-scorer BLEU newstest2015.ende.de < newstest2015.single.out

run-scorer: run_scorer.cpp
	$(CXX) -o $@ $? -I.. -L../build/ -lmoses_scorers -lz -lboost_filesystem -lboost_system

clean:
	rm -f run-scorer

.PHONY: clean test

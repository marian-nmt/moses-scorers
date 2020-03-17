/*********************************
tercpp: an open-source Translation Edit Rate (TER) scorer tool for Machine Translation.

Copyright 2010-2013, Christophe Servan, LIUM, University of Le Mans, France
Contact: christophe.servan@lium.univ-lemans.fr

The tercpp tool and library are free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the licence, or
(at your option) any later version.

This program and library are distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
**********************************/
#ifndef _TERCPPTERCALC_H___
#define _TERCPPTERCALC_H___

#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>
#include "alignmentStruct.h"
#include "bestShiftStruct.h"
#include "hashMap.h"
#include "hashMapInfos.h"
#include "hashMapStringInfos.h"
#include "terAlignment.h"
#include "terShift.h"
#include "tools.h"

using namespace TERCPPNS_Tools;
using namespace TERCPPNS_HashMapSpace;

namespace TERCPPNS_TERCpp {
// typedef size_t WERelement[2];
// Vecteur d'alignement contenant le hash du mot et son evaluation (0=ok, 1=sub, 2=ins, 3=del)
typedef std::vector<terShift> vecTerShift;
/**
  @author
*/
class terCalc {
private:
  // Vecteur d'alignement contenant le hash du mot et son evaluation (0=ok, 1=sub, 2=ins, 3=del)
  WERalignment l_WERalignment;
  // HashMap contenant les valeurs de hash de chaque mot
  hashMap bagOfWords;
  int TAILLE_PERMUT_MAX;
  int NBR_PERMUT_MAX;
  // Increments internes
  int NBR_SEGS_EVALUATED;
  int NBR_PERMUTS_CONSID;
  int NBR_BS_APPELS;
  int DIST_MAX_PERMUT;
  int CALL_TER_ALIGN;
  int CALL_CALC_PERMUT;
  int CALL_FIND_BSHIFT;
  int MAX_LENGTH_SENTENCE;
  bool PRINT_DEBUG;

  int shift_cost;
  int insert_cost;
  int delete_cost;
  int substitute_cost;
  int match_cost;
  double infinite;

  // Utilisés dans minDistEdit et ils ne sont pas réajustés
  std::vector<std::vector<double> >* S;
  std::vector<std::vector<char> >* P;
  std::vector<vecInt> refSpans;
  std::vector<vecInt> hypSpans;
  int TAILLE_BEAM;

public:
  terCalc(int beamWidth = 20, 
          int maxShiftDistance = 50, 
          int matchCost = 0, 
          int deleteCost = 1, 
          int substituteCost = 1, 
          int insertCost = 1, 
          int shiftCost = 1);

  ~terCalc();
  //             size_t* hashVec ( std::vector<std::string> s );
  void setDebugMode(bool b);
  //             int WERCalculation ( size_t * ref, size_t * hyp );
  //             int WERCalculation ( std::vector<std::string> ref, std::vector<std::string> hyp );
  //             int WERCalculation ( vector<int> ref, vector<int> hyp );
  terAlignment WERCalculation(std::vector<std::string>& hyp, std::vector<std::string>& ref);
  // 	string vectorToString(std::vector<std::string> vec);
  // 	std::vector<std::string> subVector(std::vector<std::string> vec, int start, int end);
  hashMapInfos createConcordMots(std::vector<std::string>& hyp, std::vector<std::string>& ref);
  terAlignment minimizeDistanceEdition(std::vector<std::string>& hyp,
                                       std::vector<std::string>& ref,
                                       std::vector<vecInt>& curHypSpans);
  void minimizeDistanceEdition(std::vector<std::string>& hyp,
                               std::vector<std::string>& ref,
                               std::vector<vecInt>& curHypSpans,
                               terAlignment* l_terAlign);
  //             terAlignment minimizeDistanceEdition ( std::vector<std::string>& hyp, std::vector<std::string>& ref,
  //             vector<vecInt>& curHypSpans );
  bool trouverIntersection(vecInt& refSpan, vecInt& hypSpan);
  terAlignment TER(std::vector<std::string>& hyp, std::vector<std::string>& ref, float avRefLength);
  terAlignment TER(std::vector<std::string>& hyp, std::vector<std::string>& ref);
  terAlignment TER(std::vector<int>& hyp, std::vector<int>& ref);
  bestShiftStruct* findBestShift(std::vector<std::string>& cur,
                                 std::vector<std::string>& hyp,
                                 std::vector<std::string>& ref,
                                 hashMapInfos& rloc,
                                 TERCPPNS_TERCpp::terAlignment& med_align);
  void calculateTerAlignment(terAlignment& align,
                             std::vector<bool>* herr,
                             std::vector<bool>* rerr,
                             std::vector<int>* ralign);
  vector<vecTerShift>* calculerPermutations(std::vector<std::string>& hyp,
                                            std::vector<std::string>& ref,
                                            hashMapInfos& rloc,
                                            TERCPPNS_TERCpp::terAlignment& align,
                                            std::vector<bool>* herr,
                                            std::vector<bool>* rerr,
                                            std::vector<int>* ralign);
  alignmentStruct permuter(std::vector<std::string>& words, terShift& s);
  alignmentStruct permuter(std::vector<std::string>& words, terShift* s);
  alignmentStruct permuter(std::vector<std::string>& words, int start, int end, int newloc);
};

}  // namespace TERCPPNS_TERCpp

#endif

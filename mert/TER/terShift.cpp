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
#include "terShift.h"

using namespace std;
namespace TERCPPNS_TERCpp {

// 	terShift::terShift()
// 	{
// // 		vector<string> ref;
// // 		vector<string> hyp;
// // 		vector<string> aftershift;
//
// 		//   terShift[] allshifts = null;
//
// 		numEdits=0;
// 		numWords=0;
// 		bestRef="";
//
// 		numIns=0;
// 		numDel=0;
// 		numSub=0;
// 		numSft=0;
// 		numWsf=0;
// 	}
terShift::terShift() {
  start = 0;
  end = 0;
  moveto = 0;
  newloc = 0;
  cost = 1.0;
  shifted.clear();
  alignment.clear();
  aftershift.clear();
}
terShift::terShift(int _start, int _end, int _moveto, int _newloc) {
  start = _start;
  end = _end;
  moveto = _moveto;
  newloc = _newloc;
  cost = 1.0;
}

terShift::terShift(int _start, int _end, int _moveto, int _newloc, vector<string> _shifted) {
  start = _start;
  end = _end;
  moveto = _moveto;
  newloc = _newloc;
  shifted = _shifted;
  cost = 1.0;
}
void terShift::set(terShift l_terShift) {
  start = l_terShift.start;
  end = l_terShift.end;
  moveto = l_terShift.moveto;
  newloc = l_terShift.newloc;
  shifted = l_terShift.shifted;
  //         alignment=l_terShift.alignment;
  //         aftershift=l_terShift.aftershift;
}
void terShift::set(terShift *l_terShift) {
  start = l_terShift->start;
  end = l_terShift->end;
  moveto = l_terShift->moveto;
  newloc = l_terShift->newloc;
  shifted = l_terShift->shifted;
  //         alignment=l_terShift->alignment;
  //         aftershift=l_terShift->aftershift;
}

void terShift::erase() {
  start = 0;
  end = 0;
  moveto = 0;
  newloc = 0;
  cost = 1.0;
  shifted.clear();
  alignment.clear();
  aftershift.clear();
}

// 	string terShift::vectorToString(vector<string> vec)
// 	{
// 		string retour("");
// 		for (vector<string>::iterator vecIter=vec.begin();vecIter!=vec.end(); vecIter++)
// 		{
// 			retour+=(*vecIter)+"\t";
// 		}
// 		return retour;
// 	}

string terShift::toString() {
  stringstream s;
  s.str("");
  s << "[" << start << ", " << end << ", " << moveto << "/" << newloc << "]";
  if((int)shifted.size() > 0) {
    s << " (" << vectorToString(shifted) << ")";
  }
  //         s<< endl;
  //         if ( ( int ) shifted.size() > 0 )
  //         {
  //             s << " (" << vectorToString ( alignment ) << ")";
  //         }
  //         s<< endl;
  //         if ( ( int ) shifted.size() > 0 )
  //         {
  //             s << " (" << vectorToString ( aftershift ) << ")";
  //         }
  return s.str();
}

/* The distance of the shift. */
int terShift::distance() {
  if(moveto < start) {
    return start - moveto;
  } else if(moveto > end) {
    return moveto - end;
  } else {
    return moveto - start;
  }
}

bool terShift::leftShift() {
  return (moveto < start);
}

int terShift::size() {
  return (end - start) + 1;
}
// 	terShift terShift::operator=(terShift t)
// 	{
//
// 		return t;
// 	}

}  // namespace TERCPPNS_TERCpp

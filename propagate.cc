#include "propagate.h"
#include "data.h"
#include "particle.h"
#include <iostream>

void calcAcc(yaNC::Snapshot& snap) {
  int iter = 0;

  
  //Loop Structure
  for (int i=0; i != snap.getNumber(); ++i){
    //For every i, we want to loop over every j > i
    for (int j=i+1; j != snap.getNumber(); j++ ){
      // There will be n(n-1)/2 iterations of this loop
      iter ++;
    }
  }

  std::cout << "Number of iterations of inner loop: " << iter << std::endl;
}

#include "snapshot.h"
#include "particle.h"
#include <stringx>


namespace yaNC {

  void calcAccPot(yaNC::Snapshot&, double);
  void calcAcc(yaNC::Snapshot&, double);
  void propagate(yaNC::Snapshot&, double, double);
  std::string getUsage();
}




#include "snapshot.h"

namespace yaNC {

  struct SimulationHealth {
    double dE, dL, dP, dVW;
  };

  std::ostream&operator<< (std::ostream&out, SimulationHealth const &health){
    out << "dE: " << health.dE << '\n'
	<< "dL: " << health.dL << '\n'
	<< "dP: " << health.dP << '\n'
	<< "dVW: " << health.dVW << std::endl;
    return out;
  }
  
  double testConvergance(const Snapshot&);
  SimulationHealth testHealth(const Snapshot&, double, double);
  double testSystem(Snapshot&, Snapshot&, unsigned, double, double);
  




}





#include "test.h"
#include "integrator.h"
#include "snapshot.h"
#include <iostream>
#include <cmath>


double yaNC::testConvergance(const Snapshot&snap){

  std::vector<double> fract_diffs = std::vector<double>();
  fract_diffs.push_back(1000.);
  
  double softening = 1e-2;
  double time = 1;
  double time_increment = 1;

  unsigned n = 0;
  while(n != 20 && std::abs(fract_diffs.back() - fract_diffs[fract_diffs.size() -2]) > 5e-5){

    Snapshot current_snap = Snapshot(snap);
    double current_diff = 0;

    unsigned n_iter = (unsigned) time/time_increment;
    for(unsigned i = 0; i!= n_iter; ++i){
      propagate(current_snap, time_increment, softening);
    }
    
    time_increment /= 2;

    if(fract_diffs.size() != 0){
    
      auto pnew = current_snap.begin();
      auto pold = snap.begin();
      while(pnew != current_snap.end() && pold != snap.end()){
	Point diff = pnew->pos - pold->pos;
	current_diff += abs(diff);
	
	++pnew;
	++pold;
      }
      current_diff /= current_snap.getNumber();
      fract_diffs.push_back(std::abs(current_diff));
    }
    ++n;
  }
  if(n == 20){
    return time_increment * -1;
  }else {
    return time_increment;
  }
}

yaNC::SimulationHealth yaNC::testHealth(const Snapshot& snap, double time_increment, double soft) {
  Snapshot snapCopy = snap;
  calcAccPot(snapCopy, soft);
  double time = 1;
  unsigned increments = (unsigned) time/time_increment;

  SimulationHealth health = SimulationHealth();

  double ef, eh = 0, lf, lh = 0, pf, ph = 0, VWf, VWh = 0;

  ef = snapCopy.kineticEnergy() + snapCopy.potentialEnergy();
  lf = snapCopy.angularMomentum();
  pf = snapCopy.momentum();
  VWf = snapCopy.potentialEnergy() / snapCopy.virial();
  

  for(unsigned i=0;i!=increments;++i){
    propagate(snapCopy, time_increment, soft);
    
    double tmp;
    tmp = std::abs(snapCopy.kineticEnergy() + snapCopy.potentialEnergy() - ef);
    if(tmp > eh){
      eh = tmp;
    }
    tmp = std::abs(snapCopy.angularMomentum() - lf);
    if(tmp > lh){
      lh = tmp;
    }
    tmp = std::abs(snapCopy.momentum() - pf);
    if(tmp > ph){
      ph = tmp;
    }
    tmp = std::abs((snapCopy.potentialEnergy() / snapCopy.virial()) - VWf);
    if(tmp > VWh){
      VWh = tmp;
    }
  }

  health.dE = eh/ef;
  health.dL = lh/lf;
  health.dP = ph/pf;
  health.dVW = VWh/VWf;
  
  return health;
}


double yaNC::testSystem(Snapshot&init, Snapshot&final, unsigned n, double dt, double soft){
  if(init.getNumber() != final.getNumber()){
    return -1.0;
  }
  calcAccPot(init, soft);
  

  for(unsigned i=0;i!=n;++i){
    propagate(init, dt, soft);
  }


  double dp = 0., dv = 0.;
    
  for(unsigned j=0;j!=init.getNumber();++j){
    auto ini = init.getParticle(j), fin = final.getParticle(j);

    dp += abs((fin.pos - ini.pos)/fin.pos);
    dv += abs((fin.vel - ini.vel)/fin.vel);
  }

  dp /= init.getNumber();
  dv /= init.getNumber();

  return dp+dv;
}

int main() {
  std::ifstream in("data.dat");
  yaNC::Snapshot snap = yaNC::Snapshot(0);
  snap.read(in);
  in.close();
  
  double dt =  testConvergance(snap);

  if (dt < 0){
    std::cout << "Test Failed: The propagator has not converged." << std::endl;
    return 2;
  }

  std::cout << "dT required for concordance to 5e-5: " << dt << std::endl;
  
  yaNC::SimulationHealth health = testHealth(snap, dt, 1e-2);
  std::cout << "The diagnostic information for this test was:\n"
  	    << health << std::endl;  

  
  in.open("solar.dat");
  yaNC::Snapshot solarInit = yaNC::Snapshot(0);
  solarInit.read(in);
  in.close();

  in.open("solar_final.dat");
  yaNC::Snapshot solarFinal = yaNC::Snapshot(0);
  solarFinal.read(in);
  in.close();

  double dPV = yaNC::testSystem(solarInit, solarFinal, 2556800, 0.007058324776, 1e-200);
  std::cout << "The fractional difference between the simulated system and the known solution was:\n"
	    << dPV << std::endl;

  
}

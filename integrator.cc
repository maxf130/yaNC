#include "integrator.h"
#include "snapshot.h"
#include "particle.h"

/**
 * /brief Calculates the acceleration and potentials on each particle, and writes this to each particle
 * /author Maximilian Friedersdorff
 */
void yaNC::calcAccPot(yaNC::Snapshot& snap, const double soft) {
  const double softsq = soft*soft;

  //Reset gravity for every particle
  for(auto&x:snap){
    x.acc = {0.,0.,0.};
    x.pot = 0.;
  }
  for(auto i = snap.begin();i != snap.end(); ++i){
    for(auto j = i+1;j != snap.end();++j){

      auto x = i->pos - j->pos; //3
      auto q = 1./(norm(x) + softsq); //7
      auto p = std::sqrt(q); //1
      q *= p;
      x *= q;

      i->acc -= x * j->mass; //6
      j->acc += x * i->mass; //6

      i->pot -= j->mass*p; //2
      j->pot -= i->mass*p; //2
    }
  }
}

/**
 * /brief Uses kick, drift, kick to advance the simulation by a time increment inc
 * /Author Maximilian Friedersdorff
 */
void yaNC::propagate(yaNC::Snapshot&snap, const double dt, double soft) {
  const auto dth=0.5*dt;
  for(auto&x:snap){
    x.vel += dth*x.acc;
    x.pos += dt*x.vel;
  }
  snap.incTime(dt);
  
  yaNC::calcAccPot(snap, soft);
  
  for(auto&x:snap){
    x.vel += dth*x.acc;
  }
}

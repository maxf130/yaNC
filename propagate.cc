#include "propagate.h"
#include "snapshot.h"
#include "particle.h"
#include <iostream>


/**
 * /brief Calculates the acceleration and potentials on each particle, and writes this to each particle
 * /author Maximilian Friedersdorff
 */
void yaNC::calcAccPot(yaNC::Snapshot& snap, double soft) {
  const double G = 6.67e-11;

  yaNC::Point accelerations [snap.getNumber()];
  double potentials[snap.getNumber()];

  //Loop Structure
  for (int i=0; i != snap.getNumber(); ++i){
    //For every i, we want to loop over every j > i
    for (int j=i+1; j != snap.getNumber(); j++ ){
      // Calc acceleration of particle i on j, and j on i and potentials
      yaNC::Particle&pi = snap.getParticle(i);
      yaNC::Particle&pj = snap.getParticle(j);



      yaNC::Point dist = pi.pos - pj.pos;
      double effDist_sqrt = std::sqrt(norm(dist) + soft*soft);
      yaNC::Point acceli = pj.mass * dist / effDist_sqrt*effDist_sqrt*effDist_sqrt;
      yaNC::Point accelj = acceli * -1;
      double poti = pj.mass*pj.mass / effDist_sqrt;
      double potj = pi.mass*pi.mass / effDist_sqrt;
      // Write accelerations to array
      accelerations[i] += acceli;
      accelerations[j] += accelj;
      potentials[i] += poti;
      potentials[j] += potj;
    }
    // Accelerations now containts the accelerations of all particles <= i, can write to snap
    snap.getParticle(i).acc = accelerations[i] * -G;
    // Similarly potentatials now contains the potential at <=i, can write to snap
    snap.getParticle(i).pot = potentials[i] * -G;
  }
}


/**
 * /brief Uses kick, drift, kick to advance the simulation by a time increment inc
 * /Author Maximilian Friedersdorff
 */
void yaNC::propagate(yaNC::Snapshot&snap, double inc, double soft) {
  for(int i = 0; i != snap.getNumber(); ++i){
    yaNC::Particle&pi = snap.getParticle(i);
    pi.vel += 0.5*inc*pi.acc;
    pi.pos += inc*pi.vel;
  }

  yaNC::calcAccPot(snap, soft);
  
  for(int i=0;i != snap.getNumber(); ++i){
    yaNC::Particle&pi = snap.getParticle(i);
    pi.vel += 0.5*inc*pi.acc;
  }
}

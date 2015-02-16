#include "snapshot.h"
#include "particle.h"
#include <iostream>
#include <fstream>


int main() {
  yaNC::Snapshot snapshot = yaNC::Snapshot(0);

  std::ifstream in ("data.dat");
  snapshot.read(in);


  double mpos, mvel, macc, mmass, mpot;
  for(int i =0; i!= snapshot.getNumber(); ++i){
    const yaNC::Particle& particle = snapshot.getParticle(i);
    mpos += yaNC::abs(particle.pos);
    mvel += yaNC::abs(particle.vel);
    macc += yaNC::abs(particle.acc);
    mmass += particle.mass;
    mpot += particle.pot;
  }
  int n = snapshot.getNumber();
  
  std::cout << "Mean Pos: " << mpos/n << '\n';
  std::cout << "Mean Vel: " << mvel/n << '\n';
  std::cout << "Mean Acc: " << macc/n << '\n';
  std::cout << "Mean Mass: " << mmass/n << '\n';
  std::cout << "Mean Pot: " << mpot/n << std::endl;
  
}

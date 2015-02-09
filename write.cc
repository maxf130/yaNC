#include "particle.h"
#include "data.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


int main(){
  int n = 10000;
  yaNC::Snapshot snapshot = yaNC::Snapshot(n);

  double mpos, mvel, macc, mmass, mpot;
  
  std::srand(std::time(0));
  for(int i = 0;i!=n;++i){
    
    yaNC::Particle particle = yaNC::Particle();

    particle.pos[0] = std::rand()/1e6;
    particle.pos[1] = std::rand()/1e6;
    particle.pos[2] = std::rand()/1e6;

    particle.vel[0] = std::rand()/1e6;
    particle.vel[1] = std::rand()/1e6;
    particle.vel[2] = std::rand()/1e6;

    particle.acc[0] = std::rand()/1e6;
    particle.acc[1] = std::rand()/1e6;
    particle.acc[2] = std::rand()/1e6;

    particle.mass = std::rand()/1e6;
    particle.pot = std::rand()/1e6;

    snapshot.push_back(particle);

    mpos += yaNC::abs(particle.pos);
    mvel += yaNC::abs(particle.vel);
    macc += yaNC::abs(particle.acc);
    mmass += particle.mass;
    mpot += particle.pot;
  }

  std::cout << "Mean Pos: " << mpos/n << '\n';
  std::cout << "Mean Vel: " << mvel/n << '\n';
  std::cout << "Mean Acc: " << macc/n << '\n';
  std::cout << "Mean Mass: " << mmass/n << '\n';
  std::cout << "Mean Pot: " << mpot/n << std::endl; 

  std::ofstream out ("data.dat");
  
  snapshot.write(out);

  out.close();
}

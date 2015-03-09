#include "particle.h"
#include "snapshot.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


int main(){
  unsigned n = 10;
  yaNC::Snapshot snapshot = yaNC::Snapshot(n);


  double mpos, mvel, macc, mmass, mpot;
  
  std::srand(std::time(0));
  for(unsigned i = 0;i!=n;++i){

    
    yaNC::Particle&p = snapshot.getParticle(i);

    p.pos = {std::rand()/1e6, std::rand()/1e6, std::rand()/1e6};
    p.vel = {std::rand()/1e6, std::rand()/1e6, std::rand()/1e6};
    p.acc = {std::rand()/1e6, std::rand()/1e6, std::rand()/1e6};

    p.mass = std::rand()/1e6;
    p.pot = std::rand()/1e6;

    mpos += yaNC::abs(p.pos);
    mvel += yaNC::abs(p.vel);
    macc += yaNC::abs(p.acc);
    mmass += p.mass;
    mpot += p.pot;
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

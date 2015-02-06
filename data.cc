#include "data.h"
#include "particle.h"
#include <iostream>
#include <chrono>

void yaNC::io::SnapshotAOS::write(std::ostream&o, int i, int f)const{
  // std::chrono::system_clock clock();
  o << "#---------------------------------" <<'\n';
  //  o << "#Date Printed: " << clock.now().time_since_epoch() << '\n';
  o << "#Number of particles= " << number << '\n';
  o << "#Sim Time= " << time << '\n';
  o << "#Format= PosX PosY PosZ VelX VelY VelZ AccX AccY AccZ Mass Pott" << '\n';
  o << "#----------------------------------" << '\n';
  for(int j=i;j!=f; ++j)
    {
      o << particles[j].pos << ' ' <<  particles[j].vel << ' ' <<  particles[j].acc << ' ' << particles[j].mass << ' ' <<  particles[j].pot << std::endl;
	}
}

#include "data.h"
#include "particle.h"
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <regex>

yaNC::Snapshot::Snapshot(unsigned n){
  time = 0;
  particles = std::vector<yaNC::Particle>();
  particles.reserve(n);
}

yaNC::Snapshot::~Snapshot(){}


void yaNC::Snapshot::write(std::ostream&o, int i, int f)const{
  // std::chrono::system_clock clock();
  o << "#---------------------------------" <<'\n';
  //  o << "#Date Printed: " << clock.now().time_since_epoch() << '\n';
  o << "#Number of particles= " << getNumber() << '\n';
  o << "#Sim Time= " << time << '\n';
  o << "#Format= PosX PosY PosZ VelX VelY VelZ AccX AccY AccZ Mass Pott" << '\n';
  o << "#----------------------------------" << '\n';
  for(int j=i;j!=f; ++j)
    {
      o << particles[j].pos << ' ' <<  particles[j].vel << ' ' <<  particles[j].acc << ' ' << particles[j].mass << ' ' <<  particles[j].pot << std::endl;
    }
}


void yaNC::Snapshot::read(std::istream&in, int i, int f) {
  std::string line;

  while(std::getline(in, line)){

    std::regex commentRegex("^#");
    if(std::regex_search(line, commentRegex)){
      
    } else {
      yaNC::Particle particle = yaNC::Particle();
      
      std::istringstream ss(line);
      ss >> particle.pos >> particle.vel >> particle.acc >> particle.mass >> particle.pot;

      push_back(particle);
    }
  }
}

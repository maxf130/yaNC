#include "data.h"
#include "particle.h"
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

yaNC::Snapshot::Snapshot(unsigned n){
  time = 0;
  particles = std::vector<yaNC::Particle>();
  particles.reserve(n);
}

yaNC::Snapshot::~Snapshot(){}


void yaNC::Snapshot::write(std::ostream&o, int i, int f)const{
  o << "#---------------------------------" <<'\n';
  o << "#Date Printed=" << std::time(0) << '\n';
  o << "#N=" << getNumber() << '\n';
  o << "#t=" << time << '\n';
  o << "#Format= PosX PosY PosZ VelX VelY VelZ AccX AccY AccZ Mass Pott" << '\n';
  o << "#----------------------------------";

  for(int j=i;j!=f; ++j){
    o.precision(12);
    o << '\n'
      << particles[j].pos << ' '
      << particles[j].vel << ' '
      << particles[j].acc << ' '
      << particles[j].mass << ' '
      << particles[j].pot;
  }
  o.flush();
}


void yaNC::Snapshot::read(std::istream&in) {
  std::string line;

  while(std::getline(in, line)){

    // Initialize time to a sensible value if not found in file.
    time = 0.;
    
    if(line[0] == '#'){
      // Find number of particles and reserve space if found.
      int idx = line.find("N=");
      if (idx != -1){
	std::string nParticles = line.substr(idx + 2);
	particles.reserve(std::atoi(nParticles.c_str()));
      }

      // Find simulation time and set if found.
      idx = line.find("t=");
      if (idx != -1){
	std::string time = line.substr(idx + 2);
	time = std::atof(time.c_str());
      }
    } else {
      
      yaNC::Particle particle = yaNC::Particle();
      
      std::istringstream ss(line);
      ss.precision(12);
      ss >> particle.pos >> particle.vel >> particle.acc >> particle.mass >> particle.pot;

      push_back(particle);
    }
  }
  particles.shrink_to_fit();
}

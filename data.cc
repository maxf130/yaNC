#include "data.h"
#include "particle.h"
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

yaNC::Snapshot::Snapshot(unsigned n){
  time = 0;
  particles = std::vector<yaNC::Particle>();
  particles.reserve(n);
  for (int i = 0; i != n;++i){
    particles.push_back(yaNC::Particle());
  }
}

yaNC::Snapshot::~Snapshot(){}


void yaNC::Snapshot::write(std::ostream&o, int i, int f)const{
  const auto p = 8, w = p + 6;
  
  auto end = std::chrono::system_clock::now();
  auto end_time = std::chrono::system_clock::to_time_t(end);

  
  
  o << "#---------------------------------\n";
  o << "#Date Printed=" << std::ctime(&end_time);
  o << "#N=" << getNumber() << '\n';
  o << "#t=" << time << '\n';
  o << "#Format= PosX PosY PosZ VelX VelY VelZ AccX AccY AccZ Mass Pot\n";
  o << "#----------------------------------\n";

  for(int j=i;j!=f; ++j){
    o.precision(p);
    o << std::setw(w) << particles[j].pos << ' '
      << std::setw(w) << particles[j].vel << ' '
      << std::setw(w) << particles[j].acc << ' '
      << std::setw(w) << particles[j].mass << ' '
      << std::setw(w) << particles[j].pot << '\n' ;
  }
  o.flush();
}


void yaNC::Snapshot::read(std::istream&in) {
  std::string line;

  while(std::getline(in, line)){

    // Initialize time to a sensible value if not found in file.
    time = 0.;

    bool readn = 0, readt = 0;
    if(!readn && line[0] == '#'){
      // Find number of particles and reserve space if found.
      auto idx = line.find("N=");
      if (idx != -1){
	auto N = std::stoul(line.substr(idx + 2));
	//std::cerr << "Read N='" << line.substr(idx + 2) << "' = " << N << '\n';
	particles.reserve(N);
      }
    } else if (!readt && line[0] == '#'){
      // Find simulation time and set if found.
      auto idx = line.find("t=");
      if (idx != -1){
	//std::cerr << "Read t='" << line.substr(idx + 2) << "'\n";
	time = std::stod(line.substr(idx + 2));
      }
    } else {
      
      yaNC::Particle particle;
      
      std::istringstream ss(line);
      ss >> particle.pos >> particle.vel >> particle.acc >> particle.mass >> particle.pot;

      particles.push_back(particle);
    }
  }
  particles.shrink_to_fit();
}

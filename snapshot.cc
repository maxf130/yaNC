#include "snapshot.h"
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


void yaNC::Snapshot::write(std::ostream&o, std::string&format)const{
  const auto p = 8, w = p + 6;
  
  auto time_point = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(time_point);

  
  
  o << "#---------------------------------\n";
  o << "#Date Printed=" << std::ctime(&now_c);
  o << "#N=" << getNumber() << '\n';
  o << "#t=" << time << '\n';
  o << "#Format=" << format << '\n';
  o << "#----------------------------------\n";

  for(auto i=begin();i!=end();++i){
    o.precision(p);
    for(char& c: format){
      switch (c) {
      case 'p':
	o << std::setw(w) << i->pos;
	break;
      case 'v':
	o << std::setw(w) << i->vel;
	break;
      case 'a':
	o << std::setw(w) << i->acc;
	break;
      case 'm':
	o << std::setw(w) << i->mass;
	break;
      case 'u':
	o << std::setw(w) << i->pot;
	break;
      default:
	//This should never happen!!!!!!!!!!!!!!!
	break;
      }
      o << ' ';
    }
    o << '\n';
  }
  o.flush();
}


void yaNC::Snapshot::read(std::istream&in) {
  std::string line;

  int part_read = 0;

  time = 0.;
  std::string format;

  
  bool readn = false, readt = false, readf = false;
  while(std::getline(in, line)){
    
    if(line[0] == '#'){
      if (!readn){
	auto idx = line.find("N=");
	if (idx != -1){
	  auto N = std::stoul(line.substr(idx + 2));
	  particles.reserve(N);
	  readn = true;
	}
      } else if (!readt) {
	auto idx = line.find("t=");
	if (idx != -1){
	  time = std::stod(line.substr(idx + 2));
	  readt = true;
	}
      } else if (!readf) {
	auto idx = line.find("Format=");
	if (idx != -1){
	  format = line.substr(idx + 7);
	  readf = true;
	}
      }
    } else {
      std::istringstream ss(line);
      
      if(part_read >= particles.size()){
	yaNC::Particle particle;
	
	for(char& c:format){
	  switch (c) {
	  case 'p':  
	    ss >> particle.pos;
	    break;
	  case 'v':
	    ss >> particle.vel;
	    break;
	  case 'a':
	    ss >> particle.acc;
	    break;
	  case 'm':
	    ss >> particle.mass;
	    break;
	  case 'u':
	    ss >> particle.pot;
	    break;
	  default:
	    //This should never happen!!!!!!!!!!!!!!!
	    break;
	  }
	}	
	particles.push_back(particle);
      } else {
	yaNC::Particle&particle = particles[part_read];

	for(char& c:format){
	  switch (c) {
	  case 'p':
	    ss >> particle.pos;
	    break;
	  case 'v':
	    ss >> particle.vel;
	    break;
	  case 'a':
	    ss >> particle.acc;
	    break;
	  case 'm':
	    ss >> particle.mass;
	    break;
	  case 'u':
	    ss >> particle.pot;
	    break;
	  default:
	    //This should never happen!!!!!!!!!!!!!!!
	    break;
	  }
	}
      }
      ++part_read;
    }
  }
  particles.shrink_to_fit();
}

double yaNC::Snapshot::kineticEnergy() const{
  double total;
  for(auto&p:particles){
    total += p.mass*abs(p.vel);
  }
  return total*0.5;
}

double yaNC::Snapshot::potentialEnergy() const{
  double total;
  for(auto&p:particles){
    total += p.pot*p.mass;
  }
  return total;
}

double yaNC::Snapshot::momentum() const{
  yaNC::Point total;
  for(auto&p:particles){
    total += p.mass*p.vel;
  }
  return abs(total);
}

double yaNC::Snapshot::angularMomentum() const{
  yaNC::Point total;
  for(auto&p:particles){
    total += cross(p.pos, p.mass*p.vel);
  }
  return abs(total);
}

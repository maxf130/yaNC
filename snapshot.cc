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
  for (unsigned i = 0; i != n;++i){
    particles.push_back(yaNC::Particle());
  }
}

yaNC::Snapshot::Snapshot(const Snapshot&snap){
  time = snap.time;
  particles = snap.particles;
}

yaNC::Snapshot& yaNC::Snapshot::operator=(yaNC::Snapshot&other){
  swap(*this, other);
  return *this;
}

yaNC::Snapshot::~Snapshot(){
}


void yaNC::Snapshot::write(std::ostream&o, std::string&format)const{
  const auto p = 8;
  
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
	o << i->pos;
	break;
      case 'v':
	o << i->vel;
	break;
      case 'a':
	o << i->acc;
	break;
      case 'm':
	o << i->mass;
	break;
      case 'u':
	o << i->pot;
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

  unsigned part_read = 0;

  time = 0.;
  std::string format;

  
  bool readn = false, readt = false, readf = false;
  while(std::getline(in, line)){
    
    if(line[0] == '#'){
      if (!readn){
	auto idx = line.find("N=");
	if (idx != std::string::npos){
	  auto N = std::stoul(line.substr(idx + 2));
	  particles.reserve(N);
	  readn = true;
	}
      } else if (!readt) {
	auto idx = line.find("t=");
	if (idx != std::string::npos){
	  time = std::stod(line.substr(idx + 2));
	  readt = true;
	}
      } else if (!readf) {
	auto idx = line.find("Format=");
	if (idx != std::string::npos){
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
  double total = 0;
  for(auto const p:particles){
    total += p.mass*norm(p.vel);
  }
  return total*0.5;
}

double yaNC::Snapshot::potentialEnergy() const{
  double total = 0;
  for(auto&p:particles){
    total += p.pot*p.mass;
  }
  return total*0.5;
}

double yaNC::Snapshot::momentum() const{
  yaNC::Point total = {0,0,0};
  for(auto&p:particles){
    total += p.mass*p.vel;
  }
  return abs(total);
}

double yaNC::Snapshot::angularMomentum() const{
  yaNC::Point total = {0,0,0};
  for(auto&p:particles){
    total += cross(p.pos, p.mass*p.vel);
  }
  return abs(total);
}


double yaNC::Snapshot::virial() const {
  double total = 0;
  for(auto const p:particles){
    total += p.mass*dot(p.pos, p.acc);
  }
  return total;
}

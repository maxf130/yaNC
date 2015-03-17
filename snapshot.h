#ifndef INC_SNAPSHOT_H
#define INC_SNAPSHOT_H
#include "particle.h"
#include <iostream>
#include <fstream>
#include <vector>

/**
 * \brief The yaNC (yet another N-Body code) namespace.
 *
 * All code relating to yaNC should be placed in this namespace.
 */
namespace yaNC{    

  /**
   * \brief Array of structures implementation of Snapshot
   *
   * Internally the particle information will be stores in one
   * array, containing Particle objects. Each of these objects
   * contains all relevant information about each particle.
   */
  class Snapshot{
  private:
    double time;
    std::vector<Particle> particles;
  public:
    typedef typename std::vector<Particle>::iterator iterator;
    typedef typename std::vector<Particle>::const_iterator const_iterator;
    Snapshot(unsigned);
    Snapshot(const Snapshot&);

    void swap(Snapshot&first, Snapshot&second){
      std::swap(first.time, second.time);
      std::swap(first.particles, second.particles);
    }
    
    double getTime() const {return time;}
    void incTime(double inc){time=time+inc;}
    void write(std::ostream&, std::string&) const;
    void write(std::ostream&o){
      std::string format = "pvamu";
      write(o, format);
    }
    
    void read(std::istream&);
    
    const yaNC::Particle&getParticle(int n) const{return particles[n];}
    yaNC::Particle&getParticle(int n){return particles[n];}

    iterator begin(){return particles.begin();}
    iterator end(){return particles.end();}

    const_iterator begin() const {return particles.begin();}
    const_iterator end() const {return particles.end();}

    const_iterator cbegin() const {return particles.cbegin();}
    const_iterator cend() const {return particles.cend();}
    
    std::size_t getNumber() const{return particles.size();}

    void push_back(Particle&p){particles.push_back(p);}

    double kineticEnergy() const;
    double potentialEnergy() const;
    double momentum() const;
    double angularMomentum() const;
    double virial() const;

    Snapshot& operator=(Snapshot&);
    
    ~Snapshot();
  };
}


#endif

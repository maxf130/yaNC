#ifndef INC_DATA_H
#define INC_DATA_H
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
    Snapshot(unsigned);
    
    inline double getTime(){return time;}
    inline void incTime(double inc){time=time+inc;}
    inline void write(std::ostream&o) const{return write(o, 0, getNumber()-1);}
    void write(std::ostream&, int, int) const;
    
    void read(std::istream&);
    
    inline const yaNC::Particle&getParticle(int n) const{return particles[n];}
    inline yaNC::Particle&getParticle(int n){return particles[n];}
    
    inline const int getNumber() const{return particles.size();}

    inline void push_back(Particle&p){particles.push_back(p);}

    ~Snapshot();
  };
}


#endif

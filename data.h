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
    SnapshotAOS(unsigned);
    
    inline double getTime(){return time;}
    inline void incTime(double inc){time=time+inc;}
    inline void write(std::ostream&o) const{return write(o, 0, number-1);}
    void write(std::ostream&, int, int) const;
    
    inline void read(std::istream&i){return read(i, 0, number-1);}
    void read(std::istream&, int, int);
    
    inline const yaNC::Particle&getParticle(int n) const{return particles[n];}
    inline yaNC::Particle&getParticle(int n){return particles[n];}
    
    inline const int getNumber() const{return particles.size();}
    
    ~SnapshotAOS();
  };
}


#endif

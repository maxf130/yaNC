#ifndef INC_DATA_H
#define INC_DATA_H
#include "particle.h"
#include <iostream>
#include <fstream>

/**
 * \brief The yaNC (yet another N-Body code) namespace.
 *
 * All code relating to yaNC should be placed in this namespace.
 */
namespace yaNC{

  /**
   * \brief Contains io related elements
   *
   * yaNC:io contains the Snapshot Classes which are responsible for
   * reading and writing particle information to and from files.  They
   * are also intended as containers for particles.  There exist two
   * implementations, SnapshotSOA and SnapshotAOS, wich store the particle
   * info by Structure of Arrays and the Array of Structures models.  
   */
  namespace io{

    /**
     * \brief The abstract base class for Snapshot implementations
     *
     * Contains only abstract members and defines the interface for
     * Snapshot implementations.
     */
    class Snapshot{
    public:
      
      /**
       * \brief Writes particle info to ostream
       */
      virtual void write(std::ostream&) const =0;
      /**
       * \brief Writes particle info between to indeces to ostream
       */
      virtual void write(std::ostream&, int, int) const =0;

      /**
       * \brief Reads particle info two stream
       */
      virtual void read(std::istream&)=0;
      /**
       * \brief Reads particle info between two indeces from stream
       */
      virtual void read(std::istream&, int, int)=0;

      /**
       * \brief Provides constant access to one of the particles
       */
      virtual const yaNC::Particle&getParticle(int) const =0;
      /** 
       * \brief Provides non constant access to one of the particles
       */
      virtual void setParticle(yaNC::Particle&)=0;
    };

    /**
     * \brief Structure of Arrays implementation of Snapshot
     *
     * Internally the particle information will be stored as a 
     * series of arrays or vectors.  Ie. one array or vector for
     * masses, for velocities, positions etc.
     */
    class SnapshotSOA: public Snapshot{
    public:
      SnapshotSOA(int);

      void write(std::ostream&) const;
      void write(std::ostream&, int, int) const;

      void read(std::istream&);
      void read(std::istream&, int, int);

      const yaNC::Particle&getParticle(int) const;
      void setParticle(yaNC::Particle&);

      ~SnapshotSOA();
    };

    /**
     * \brief Array of structures implementation of Snapshot
     *
     * Internally the particle information will be stores in one
     * array, containing Particle objects. Each of these objects
     * contains all relevant information about each particle.
     */
    class SnapshotAOS: public Snapshot{
    public:
      SnapshotAOS(int);

      void write(std::ostream&) const;
      void write(std::ostream&, int, int) const;

      void read(std::istream&);
      void read(std::istream&, int, int);

      const yaNC::Particle&getParticle(int) const;
      void setParticle(yaNC::Particle&);

      ~SnapshotAOS();
    };
  }
}

#endif

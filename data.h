#ifndef INC_DATA_H
#define INC_DATA_H
#include "particle.h"
#include <iostream>
#include <fstream>

namespace yaNC{
  namespace io{
    class Snapshot{
    public:
      virtual void write(std::ostream&) const =0;
      virtual void write(std::ostream&, int, int) const =0;

      virtual void read(std::istream&)=0;
      virtual void read(std::istream&, int, int)=0;

      virtual const yaNC::Particle&getParticle(int) const =0;
      virtual void setParticle(yaNC::Particle&)=0;
    };

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

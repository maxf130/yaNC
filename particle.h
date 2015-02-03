#ifndef INC_PARTICLE_H
#define INC_PARTICLE_H

namespace yaNC {
  struct Particle{
    double mass;
    double pos_x, pos_y, pos_z;
    double vel_x, vel_y, vel_z;
  Particle(): mass(0.0), pos_x(0.0), pos_y(0.0), pos_z(0.0), vel_x(0.0), vel_y(0.0), vel_z(0.0){}
  Particle(double m, double px, double py, double pz, double vx, double vy, double vz): mass(m), pos_x(px), pos_y(py), pos_z(pz), vel_x(vx), vel_y(vy), vel_z(vz){}
  };
}


#endif

#ifndef INC_PARTICLE_H
#define INC_PARTICLE_H
#include <array>
#include <cmath>
#include <iostream>

namespace yaNC {
  typedef std::array<double, 3> Point;
  inline Point&operator+= (Point&x, Point const&y)
  {
    x[0] += y[0];
    x[1] += y[1];
    x[2] += y[2];
    return x;
  }

  inline Point&operator-= (Point&x, Point const&y)
  {
    x[0] -= y[0];
    x[1] -= y[1];
    x[2] -= y[2];
    return x;
  }

  inline Point&operator*= (Point&x, double y)
  {
    x[0] *= y;
    x[1] *= y;
    x[2] *= y;
    return x;
  }

  inline Point&operator/= (Point&x, double y)
  {
    return x *= 1/y;
  }

  inline Point operator+ (Point const&x, Point const&y)
  {
    return {x[0] + y[0], x[1] + y[1], x[2] + y[2]};
  }

  inline Point operator- (Point const&x, Point const&y)
  {
    return {x[0] - y[0], x[1] - y[1], x[2] - y[2]};
  }

  inline Point operator* (Point const&x, double y)
  {
    return {x[0] * y, x[1] * y, x[2] *y};
  }

  inline Point operator* (double y, Point const&x)
  {
    return x*y;
  }

  inline Point operator/ (Point const&x, double y)
  {
    return x * (1/y);
  }

  inline double norm (Point const&x)
  {
    return x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
  }

  inline double abs (Point const&x)
  {
    return std::sqrt(norm(x));
  }

  inline double square(double x)
  {
    return x*x;
  }
  
  inline double dist_sq(Point const&x, Point const&y)
  {
    return square(x[0] - y[0]) + square(x[1] - y[1]) + square(x[2] - y[2]);
  }

  inline std::ostream&operator<<(std::ostream&out, Point const&x)
  {
    return out << x[0] << ' ' << x[1] << ' ' << x[2]; 
  }

  inline std::istream&operator>>(std::istream&in, Point&x)
  {
    return in >> x[0] >> x[1] >> x[2];
  }
  
  struct Particle{
    double mass, pot;
    Point pos, vel, acc;
  };
}


#endif

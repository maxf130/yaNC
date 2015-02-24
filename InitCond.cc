#include "snapshot.h"
#include "particle.h"
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

int main()
{
  // Number, Mass, and Rs user input 
  unsigned number;
  cout << "Please input a positive integer number of bodies.\n";
  cin >> number;
  double mass_tot=1;
  double radius_scale=1;

  // //Random number generator
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> dis(0,1);
  // Position Calculations
  // R^2/3, where R = M(<r)
  double R = dis(gen);
   yaNC::Snapshot snap(number);
   for(int i=0; i!=number; i++ ) //For each particle calculate Position and Velocity
   {
  // Radius Calculation
  double radius = radius_scale * sqrt(

				      pow(R,(2./3.))
				      /
				      ( pow(mass_tot,2./3.) - pow(R,2./3.) )

				      );

  double pi = atan(1)*4; // Pi Defined
  // Position
  double pphi = 2*pi*dis(gen); // Random Uniform Phi [0,2pi]
  double pcostheta =2*dis(gen)-1; // Random Uniform Cos(theta)[-1,1]
  double px = radius*sqrt(1-pow(pcostheta,2))*cos(pphi);//Spherical to Cartesian X
  double py = radius*sqrt(1-pow(pcostheta,2))*sin(pphi);//Spherical to Cartesian Y
  double pz = radius*pcostheta;//Spherical to Cartesian Z
  const int G = 1; // Gravitational Const

  // Escape Velocity Calculation
  double vesc = sqrt(

		     (2*G*mass_tot)
		     / 
		     (sqrt( pow(radius,2) + pow(radius_scale,2) ))

		     );

  double f0 = pow(vesc,7);
  double vel, f;
  do{
    vel = pow(dis(gen),1./3)*vesc;
    f  = pow(pow(vesc,2)-pow(vel,2), 7./2.);
  } while ( dis(gen)*f0>f );
  // Velocity
  double vphi = 2*pi*dis(gen);
  double vcostheta = 2*dis(gen)-1;
  double vx = vel*sqrt(1-pow(vcostheta,2))*cos(vphi);
  double vy = vel*sqrt(1-pow(vcostheta,2))*sin(vphi);
  double vz = vel*vcostheta;
  // Asign values
  snap.getParticle(i).mass=mass_tot/number;
  snap.getParticle(i).pos={px,py,pz};
  snap.getParticle(i).vel={vx,vy,vz};
   }
   // Name output file
 string filename;
 cout << "Please name the output file \n";
 cin >> filename;
 // Print to file
 std::ofstream out (filename);
 snap.write(out);
 out.close();
}

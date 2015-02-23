#include "data.h"
#include "particle.h"
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

int main()
{
  // Number, Mass, and Rs user input 
  unsigned number;
  cout << "Please input the number of bodies.\n";
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

  // For each particle calculate and print to snapshot
 for(int i=0; i!=number; i++ )
   {
  // Radius Calculation
  double radius = radius_scale * sqrt(

				      pow(R,(2./3.))
				      /
				      ( pow(mass_tot,2./3.) - pow(R,2./3.) )

				      );

  cout << "Radius= " << radius << endl;
  double pi = atan(1)*4;
  double pphi = 2*pi*dis(gen);
  double pcostheta =2*dis(gen)-1;
  double px = radius*sqrt(1-pow(pcostheta,2))*cos(pphi);
  double py = radius*sqrt(1-pow(pcostheta,2))*sin(pphi);
  double pz = radius*pcostheta;
  cout << "Pos x= " << px << '\n';
  cout << "Pos y= " << py << '\n';
  cout << "Pos z= " << pz << endl;
  // Gravitational Const
  const int G = 1;
  // Escape Velocity
  double vesc = sqrt(

		     (2*G*mass_tot)
		     / 
		     (sqrt( pow(radius,2) + pow(radius_scale,2) ))

		     );

  cout << "Escape Velocity= " << vesc << endl;
  double f0 = pow(vesc,7);
  cout << "F0= " << f0 << endl;
  double vel, f;
  do{
  vel = pow(dis(gen),1./3)*vesc;
  f  = pow(pow(vesc,2)-pow(vel,2), 7./2.);
   } while ( dis(gen)*f0>f );
  cout<<vel<<" "<<f<<endl;
  double vphi = 2*pi*dis(gen);
  double vcostheta = 2*dis(gen)-1;
  double vx = vel*sqrt(1-pow(vcostheta,2))*cos(vphi);
  double vy = vel*sqrt(1-pow(vcostheta,2))*sin(vphi);
  double vz = vel*vcostheta;
  cout << "Vel x= " << vx << '\n';
  cout << "Vel y= " << vy << '\n';
  cout << "Vel z= " << vz << endl;
  yaNC::Particle P=snap.getParticle(i);
  P.mass=mass_tot/number;
  P.pos={px,py,pz};
  P.vel={vx,vy,vz};
 }
}

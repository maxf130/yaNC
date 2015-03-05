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
  unsigned nosys;
  cout << "Please input either 1 or 2 for the number of systems\n";
  cin >> nosys;
  if(nosys >2 && nosys <1)
    {
      cout << "Invalid, please input 1 or 2\n";
      return 2;
    }
  double opx,opy,opz,ovx,ovy,ovz;
  if(nosys==2)
    {
  cout << "Please input the x,y,z offset of the second system\n";
  cin >> opx >> opy >> opz;
  cout << "Please input the x,y,z velocity of the second system\n";
  cin >> ovx >> ovy >> ovz;
    } 
  const double mass_tot=1;
  const double radius_scale=1;
  const double G = 1;
  const double velocity_scale = sqrt(G*mass_tot/radius_scale);
  const double m = mass_tot/number;
  // //Random number generator
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> dis(0,1);
  // Position Calculations
  // R^2/3, where R = M(<r)
  const double pi = atan(1)*4; // Pi Defined
   yaNC::Snapshot snap(number);
   unsigned count=0;


   for(auto& part:snap) //For each particle calculate Position and Velocity
   {
     part.mass=m;
     
     
     // Radius Calculation
     double R = pow(dis(gen), 0.66666666666666666666666666666666666666666667);
     double x = sqrt(R/(1-R));
     double radius = radius_scale * x;
     
     // Position
     double pphi = 2*pi*dis(gen); // Random Uniform Phi [0,2pi]
     double pcostheta =2*dis(gen)-1; // Random Uniform Cos(theta)[-1,1]
     double psintheta = sqrt(1-pcostheta*pcostheta);
     part.pos={radius*psintheta*cos(pphi),//Spherical to Cartesian X
		  radius*psintheta*sin(pphi),//Spherical to Cartesian Y
		  radius*pcostheta };        //Spherical to Cartesian Z

     
     // Escape Velocity Calculation
     double psi = 1/sqrt(x*x+1);
     double vescsq = psi+psi;
     double vesc = sqrt(vescsq);
     
     double f0 = pow(vesc,7);
     double vel, f;
     do{
       vel = cbrt(dis(gen))*vesc;
       f  = pow(vescsq-vel*vel, 3.5);
     } while ( dis(gen)*f0>f );
     // Velocity
     vel *= velocity_scale;
     double vphi = 2*pi*dis(gen);
     double vcostheta = 2*dis(gen)-1;
     double vsintheta = sqrt(1-vcostheta*vcostheta);
     part.vel={vel*vsintheta*cos(vphi),//Spherical to Cartesian X
		  vel*vsintheta*sin(vphi),//Spherical to Cartesian Y
		  vel*vcostheta };        //Spherical to Cartesian Z
     if(nosys==2 && count>((number/2)-1)){

       part.pos[0] += opx;
       part.pos[1] += opy;
       part.pos[2] += opz;
       part.vel[0] += ovx;
       part.vel[1] += ovy;  
       part.vel[2] += ovz;       
     }
     ++count;
          
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

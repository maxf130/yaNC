#include <iostream>
#include <cmath>
#include <random>
using namespace std;

int main()
{
  double pi = 3.14159265;
  // Number, Mass, and Rs user input
  double number;
  cout << "Please input the number of bodies.\n";
  cin >> number;
  double mass_tot;
  cout << "Please input total mass of the system.\n";
  cin >> mass_tot;
  double radius_scale;
  cout << "Please input Scale Radius.\n";
  cin >> radius_scale;
  cout << number << ' ' <<  mass_tot << ' ' << radius_scale << endl;

  // //Random number generator
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> disR(0, mass_tot);
  uniform_real_distribution<double> disphi(0,2*pi);
  uniform_real_distribution<double> distheta(0,pi);
   
  //(1) R^2/3, where R = M(<r)
  double R = disR(gen);
  cout << "R= " << R << endl;
  //(2) Radius Calculation
  double f = pow(R,(2./3.));
  cout << "f= " << f << endl;
  // M^2/3
  double g = pow(mass_tot,(2./3.));
  cout <<"g= " << g << endl;
  // z
  double h = f/(g-f);
  cout <<"h= " << h << endl;
  // r
  double radius = radius_scale * pow(h,0.5);
  cout << "Radius= " << radius << endl;
  
  //(3) Random azimuth phi [0,2pi]
  double phi = disphi(gen);
  cout << "Phi= " << phi << endl;
  //(4) Random theta [0,pi]
 double theta = distheta(gen);
  cout << "Theta= " << theta << endl;
  //(5) Set x,y,z
  double x = radius*sin(theta)*cos(phi);
  double y = radius*sin(theta)*sin(phi);
  double z = radius*cos(theta);
  cout << "x= " << x << '\n';
  cout << "y= " << y << '\n';
  cout << "z= " << z << endl;
}

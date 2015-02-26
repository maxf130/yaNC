#include "propagate.h"
#include "snapshot.h"
#include "particle.h"
#include <iostream>


/**
 * /brief Calculates the acceleration and potentials on each particle, and writes this to each particle
 * /author Maximilian Friedersdorff
 */
void yaNC::calcAccPot(yaNC::Snapshot& snap, const double soft) {
  const double softsq = soft*soft;

  //Reset gravity for every particle
  for(auto&x:snap){
    x.acc = {0.,0.,0.};
    x.pot = 0.;
  }
  for(auto i = snap.begin();i != snap.end(); ++i){
    for(auto j = i+1;j != snap.end();++j){

      yaNC::Point dist = i->pos - j->pos; //3
      double reciprocal_eff_dist = 1./(norm(dist) + softsq); //7
      double reciprocal_eff_dist_sq = std::sqrt(reciprocal_eff_dist); //1

      yaNC::Point acc_mod_mass = dist*(reciprocal_eff_dist*reciprocal_eff_dist_sq); //4
      i->acc -= acc_mod_mass * j->mass; //6
      j->acc += acc_mod_mass * i->mass; //6

      i->pot -= j->mass*reciprocal_eff_dist_sq; //2
      j->pot -= i->mass*reciprocal_eff_dist_sq; //2
    }
  }
}

/**
 * /brief Uses kick, drift, kick to advance the simulation by a time increment inc
 * /Author Maximilian Friedersdorff
 */
void yaNC::propagate(yaNC::Snapshot&snap, const double dt, double soft) {
  const auto dth=0.5*dt;
  for(auto&x:snap){
    x.vel += dth*x.acc;
    x.pos += dt*x.vel;
  }
  snap.incTime(dt);
  
  yaNC::calcAccPot(snap, soft);
  
  for(auto&x:snap){
    x.vel += dth*x.acc;
  }
}



std::string yaNC::getUsage(){
  std::string returnValue = "Usage: propagate PARAMETERS... INPUT OUTPUT \n"
    "\n"
    "Parameters: \n"
    "    -n        The number of iterations to run \n"
    "    -t        The time increment per iteration.  Should be a float of form XeY, where X and Y are"
        " integers \n"
    "    -p        The number of iterations between printing the current state to file \n"
    "    -s        The softening length.  Should be greater than 1"
    "\n"
    "INPUT is the file from which to read the initial state.  OUTPUT is the pattern that \n"
    "the output file should take.  The file extention should not be included.";
  return returnValue;
}




int main(int argc, char*argv[]) {
  int iter;
  int printIter;
  double inc, soft;
  std::string in, out;

  //Check for correct number of arguments (every parameter must be set explicitly)
  if(argc != 11){
    std::cout << yaNC::getUsage() << std::endl;
    return 2;
  }
  for(int i=1;i!=argc;++i){
    std::string argument(argv[i]);

    if (i == argc -2){
      // This should be the input file for the propagator
      in = std::string(argv[i]);
    } else if (i == argc -1){
      // This should be the output pattern for the propagator
      out = std::string(argv[i]);
    } else if (argument == "-n"){
      iter = std::stoul(argv[++i]);
    } else if (argument == "-t"){
      inc = std::stod(argv[++i]);
    } else if (argument == "-p"){
      printIter = std::stoul(argv[++i]);
    } else if (argument == "-s"){
      soft = std::stod(argv[++i]);
      if (soft < 0){
	std::cout << "The softening length must be greater than 0 \n\n" << yaNC::getUsage() << std::endl;
      }
    }
  }
  std::cout << "Arguments given: \n"
	    << " Number of iterations: " << iter << '\n'
	    << " Number of iterations between printing: " << printIter << '\n'
	    << " Time increment: " << inc << '\n'
	    << " Softening length: " << soft << '\n'
	    << " Input, output file pattern: " << in << ", " << out << std::endl;

  //Check that input file exists.
  std::ifstream input(in);

  if(!input.good()){
    std::cout << in << ": No such file or directory" << std::endl;
    return 2;
  }
  //File is good.  Read into snapshot
  yaNC::Snapshot snapshot(0);
  snapshot.read(input);


  //Perform propagation
  for(int i=0; i!=iter; ++i){
    propagate(snapshot, inc, soft);

    //If i is multiple of printIter, print out the result
    if(i%printIter == 0){
      std::ofstream output(out + std::to_string(i/printIter) + ".dat");
      snapshot.write(output);
      output.close();
    }
  }
  std::ofstream output(out + "_final.dat");
  snapshot.write(output);
  output.close();
}

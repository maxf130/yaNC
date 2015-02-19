#include "propagate.h"
#include "snapshot.h"
#include "particle.h"
#include <iostream>


/**
 * /brief Calculates the acceleration and potentials on each particle, and writes this to each particle
 * /author Maximilian Friedersdorff
 */
void yaNC::calcAccPot(yaNC::Snapshot& snap, double soft) {
  const double G = 6.67e-11;

  yaNC::Point accelerations [snap.getNumber()];
  double potentials[snap.getNumber()];

  //Loop Structure
  for (int i=0; i != snap.getNumber(); ++i){
    //For every i, we want to loop over every j > i
    for (int j=i+1; j != snap.getNumber(); j++ ){
      // Calc acceleration of particle i on j, and j on i and potentials
      yaNC::Particle&pi = snap.getParticle(i);
      yaNC::Particle&pj = snap.getParticle(j);



      yaNC::Point dist = pi.pos - pj.pos; //3 FLOPS
      double effDist_sqrt = 1./std::sqrt(norm(dist) + soft*soft); //7 FLOPS

      yaNC::Point acceli =  dist * (pj.mass*effDist_sqrt*effDist_sqrt*effDist_sqrt); //6FLOPS
      yaNC::Point accelj = acceli * -1; //1 FLOP
      double poti = pj.mass*pj.mass * effDist_sqrt; //2FLOPS
      double potj = pi.mass*pi.mass * effDist_sqrt; //2FLOPS
      // Write accelerations to array
      accelerations[i] += acceli; //3 FLOPS
      accelerations[j] += accelj; //3 FLOPS
      
      potentials[i] += poti; //1FLOP
      potentials[j] += potj; //1FLOP
    }
    // Accelerations now containts the accelerations of all particles <= i, can write to snap
    snap.getParticle(i).acc = accelerations[i] * -G;
    // Similarly potentatials now contains the potential at <=i, can write to snap
    snap.getParticle(i).pot = potentials[i] * -G;
  }
}

/**
 * /brief Uses kick, drift, kick to advance the simulation by a time increment inc
 * /Author Maximilian Friedersdorff
 */
void yaNC::propagate(yaNC::Snapshot&snap, double inc, double soft) {
  for(int i = 0; i != snap.getNumber(); ++i){
    yaNC::Particle&pi = snap.getParticle(i);
    pi.vel += 0.5*inc*pi.acc;
    pi.pos += inc*pi.vel;
  }

  yaNC::calcAccPot(snap, soft);
  
  for(int i=0;i != snap.getNumber(); ++i){
    yaNC::Particle&pi = snap.getParticle(i);
    pi.vel += 0.5*inc*pi.acc;
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
  yaNC::Snapshot snapshot(1);
  snapshot.read(input);


  //Perform propagation
  for(int i=0; i!=iter; ++i){
    std::cout<< "Performing iteration no: " << i << std::endl;
    propagate(snapshot, inc, soft);

    //If i is multiple of printIter, print out the result
    if(i%printIter == 0){
      std::ofstream output(out + std::to_string(i/printIter) + ".dat");
      snapshot.write(output);
      output.close();
    }
  }
}

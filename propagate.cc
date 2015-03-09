#include "propagate.h"
#include "integrator.h"
#include "snapshot.h"
#include "particle.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>






int main(int argc, char*argv[]) {
  int iter;
  int printIter;
  double inc, soft;
  std::string in, out, format_end, format, log;

  //Check for correct number of arguments (every parameter must be set explicitly)
  if(argc != 2){
    std::cout << yaNC::getUsage() << std::endl;
    return 2;
  }

  std::ifstream optsInput(argv[1]);
  std::unordered_map<std::string, std::string> opts = yaNC::getOptions(optsInput);
  optsInput.close();

  auto value = opts.end();
  bool failed = false;
  std::string failure;


  value = opts.find("iterations");
  if(value != opts.end()){
    iter = std::stoul(std::get<1>(*value));
  } else {
    failed = true;
    failure += "'iterations' not specified. \n";
  }

  value = opts.find("print_interval");
  if(value != opts.end()){
    printIter = std::stoul(std::get<1>(*value));
  } else {
    failed = true;
    failure += "'print_interval' not specified. \n";
  }

  value = opts.find("time_increment");
  if(value != opts.end()){
    inc = std::stod(std::get<1>(*value));
  } else {
    failed = true;
    failure += "'time_increment' not specified. \n";
  }

  value = opts.find("softening_length");
  if(value != opts.end()){
    soft = std::stod(std::get<1>(*value));
  } else {
    failed = true;
    failure += "'softening_length' no specified. \n";
  }

  value = opts.find("input_file");
  if(value != opts.end()){
    in = std::get<1>(*value);
  } else {
    failed = true;
    failure += "'input_file' not specified. \n";
  }

  value = opts.find("output_pattern");
  if(value != opts.end()){
    out = std::get<1>(*value);
  } else {
    failed = true;
    failure += "'output_pattern' not specified. \n";
  }

  value = opts.find("print_format_end");
  if(value != opts.end()){
    format_end = std::get<1>(*value);
  } else {
    failed = true;
    failure += "'print_format_end' not specified. \n";
  }

  value = opts.find("print_format");
  if(value != opts.end()){
    format = std::get<1>(*value);
  } else {
    failed = true;
    failure += "'print_format' not specified. \n";
  }


  value = opts.find("log");
  if(value != opts.end()){
    log = std::get<1>(*value);
  } else {
    failed = true;
    failure += "'log' not specified. \n";
  }

  

  
  if(failed){
    std::cout << failure;
    std::cout.flush();
    return 2;
  }


  //Check that input file exists.
  std::ifstream input(in);

  if(!input.good()){
    std::cout << in << ": No such file or directory" << std::endl;
    return 2;
  }
  //File is good.  Read into snapshot
  yaNC::Snapshot snapshot(0);
  snapshot.read(input);
  input.close();

  std::ofstream logstream(log);
  logstream << "#SimTime Energe Kinectic Potential Virial VirialRatio Momentum AngularMomentum CPUTime TotalCPUTime"  << '\n';

  
  std::clock_t begin = std::clock();
  //Perform propagation
  for(int i=0; i!=iter; ++i){
    std::clock_t start = std::clock();
    propagate(snapshot, inc, soft);

    //If i is multiple of printIter, print out the result
    if(i%printIter == 0){
      std::ofstream output(out + std::to_string(i/printIter) + ".dat");
      snapshot.write(output, format);
      output.close();
    }
    std::clock_t end = std::clock();
    yaNC::writeLog(logstream, snapshot, end-start, end-begin);
  }
  std::ofstream output(out + "_final.dat");
  snapshot.write(output, format_end);
  output.close();
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


std::unordered_map<std::string, std::string> yaNC::getOptions(std::istream& in){
  std::string line;
  std::unordered_map<std::string, std::string> opts;
  
  while(std::getline(in, line)){
    if(line[0] == '#'){
      continue;
    }
    int idx = line.find("=");
    if (idx == -1){
      continue;
    }
    line.replace(idx, 1, " ");
    std::istringstream ss(line);

    std::string key,value;
    ss >> key >> value;
    opts.emplace(key, value);
  }
  return opts;
}

void yaNC::writeLog(std::ostream&out, const yaNC::Snapshot&snap, double ttot, double t){
  double p = 8;
  double w = p + 6;

  
  //Write header
  

  out << std::setprecision(p);
  const auto T = snap.kineticEnergy(), W = snap.virial(), V = snap.potentialEnergy();
  out << std::setw(w) << snap.getTime() << ' '
      << std::setw(w) << (T+V) << ' '
      << std::setw(w) << T << ' '
      << std::setw(w) << V << ' '
      << std::setw(w) << W << ' '
      << std::setw(w) << (-2*T/W) << ' '
      << std::setw(w) << snap.momentum() << ' '
      << std::setw(w) << snap.angularMomentum() << ' '
      << std::setw(w) << t << ' '
      << std::setw(w) << ttot << std::endl;
}

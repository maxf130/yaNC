#include "utils.h"
#include <string>
#include <sstream>
#include <iomanip>

std::string yaNC::padInt(int n, int w){
  std::stringstream ss;
  ss << std::setw(w) << std::setfill('0') << n;
  return ss.str();
}

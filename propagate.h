#include "snapshot.h"
#include "particle.h"
#include <string>
#include <unordered_map>


namespace yaNC {
  std::string getUsage();
  std::unordered_map<std::string, std::string> getOptions(std::istream&);
}





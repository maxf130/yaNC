#include "snapshot_factory.h"
#include "data.h"
using namespace yaNC;

io::Snapshot& create_snapshot(int n){
  static io::SnapshotAOS S(n);
  return S;
}

io::Snapshot& create_snapshot(){
  static io::SnapshotAOS S(0);
  return S;
}

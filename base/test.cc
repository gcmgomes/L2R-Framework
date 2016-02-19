#include <iostream>
#include <fstream>
#include "dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;
  }
  base::Dataset d;

  d.Parse(argv[1]);

  /*auto query = d.queries().begin();
  while(query != d.queries().end()) {
    auto document = query->documents().begin();
    while(document != query->documents().end()) {
      cout << document->ToString() << endl;
      ++document;
    }
    ++query;
  }*/
}

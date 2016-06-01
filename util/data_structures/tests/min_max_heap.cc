#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <cmath>
#include <unordered_set>
#include "../min_max_heap.h"

using namespace std;

struct Stupidity {
  Stupidity(int x) {
    a = x;
    b = 2*a;
    c = 2*b;
  }
  int a;
  int b;
  int c;

  string ToString() {
    stringstream str;
    str << "(" << a << ", " << b << ", " << c << ")";
    return str.str();
  }

  bool operator<(const Stupidity& o) const {
    return (a == o.a)
             ? (b == o.b) 
               ? c < o.c
               : b < o.b
             : a < o.a;
  }
};

int main(int argc, char** argv) {
  string file_path = "/var/tmp/";
  if (argc > 1) {
    file_path = argv[1];
  }
  vector<Stupidity> scores;
  scores.emplace_back(7.0);
  scores.emplace_back(8.0);
  scores.emplace_back(4.0);
  scores.emplace_back(2.0);
  scores.emplace_back(6.0);
  scores.emplace_back(6.0);
  scores.emplace_back(3.0);
  scores.emplace_back(5.0);
  scores.emplace_back(1.0);
  scores.emplace_back(9.0);
  // random_shuffle(scores.begin(), scores.end());
  util::data_structures::MinMaxHeap<Stupidity> heap;
  int i = 0;
  while (i < scores.size()) {
    heap.push(scores[i]);
    i++;
  }
  while (!heap.empty()) {
    cout << "Size: " << heap.size() << endl;
    auto min = heap.min();
    auto max = heap.max();
    cout << "Minimum: " << min.ToString() << endl;
    cout << "Maximum: " << max.ToString() << endl;
    heap.pop_max();
    heap.pop_min();
  }
  std::cout << "Done" << std::endl;
  /*heap.Print();
  cout << "Minimum: " << heap.min().score() << endl;
  cout << "Maximum: " << heap.max().score() << endl;
  heap.pop_max();
  heap.Print();*/
}

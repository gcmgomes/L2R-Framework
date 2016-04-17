#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <cmath>
#include <unordered_set>
#include "../heap.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/";
  if (argc > 1) {
    file_path = argv[1];
  }
  vector<long double> scores;
  scores.push_back(7.0);
  scores.push_back(8.0);
  scores.push_back(4.0);
  scores.push_back(2.0);
  scores.push_back(6.0);
  scores.push_back(6.0);
  scores.push_back(3.0);
  scores.push_back(5.0);
  scores.push_back(1.0);
  scores.push_back(9.0);
  random_shuffle(scores.begin(), scores.end());
  bayes::branch_and_bound::Heap heap;
  int i = 0;
  while (i < scores.size()) {
    heap.push(bayes::branch_and_bound::Graph(scores[i]));
    i++;
  }
  heap.Print();
  while (!heap.empty()) {
    cout << "Popping: " << heap.top().score() << endl;
    heap.pop();
  }
}

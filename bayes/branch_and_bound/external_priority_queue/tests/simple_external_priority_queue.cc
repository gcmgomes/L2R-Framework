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
#include "../simple_external_priority_queue.h"

using namespace std;

void print(vector<int> v) {
  auto it = v.begin();
  while(it != v.end()) {
    cout << *it << " "; 
    ++it;
  }
  cout << endl;
} 

string bitfy(unsigned x) {
  string s;
  int i = 0;
  while(i < 32) {
    if(x & 1) {
      s += '1';
    }
    else {
      s += '0';
    }
    i++;
  }
  reverse(s.begin(), s.end());
  return s;
}

int main(int argc, char** argv) {
  vector<int> scores;
  scores.push_back(1.0);
  scores.push_back(2.0);
  scores.push_back(3.0);
  scores.push_back(4.0);
  scores.push_back(5.0);
  scores.push_back(6.0);
  scores.push_back(6.0);
  scores.push_back(7.0);
  scores.push_back(8.0);
  scores.push_back(9.0);
  do {
    unsigned order = 0, limit = (1 << 10) - 1;
    print(scores);
    while (order < limit) {
      unsigned mask = 1 << 9;
      multiset<int> s_scores;
      bayes::branch_and_bound::SimpleExternalPriorityQueue heap(2, 2);
      int i = 0;
      while (i < scores.size()) {
        heap.push(scores[i]);
        s_scores.insert(scores[i]);
        i++;
      }
      while (!heap.empty()) {
        if(mask & order) {
          int val = heap.max();
          heap.pop_max();
          if(val != *s_scores.rbegin()) {
            abort();
          }
          else {
            auto it = s_scores.end();
            --it;
            s_scores.erase(it);
          }
        }
        else {
          int val = heap.min();
          heap.pop_min();
          if(val != *s_scores.begin()) {
            abort();
          }
          else {
            s_scores.erase(s_scores.begin());
          }
        }
        mask >>= 1;
      }
      order++;
    }
  } while (next_permutation(scores.begin(), scores.end()));
  std::cout << "Done" << std::endl;
  /*heap.Print();
  cout << "Minimum: " << heap.min().score() << endl;
  cout << "Maximum: " << heap.max().score() << endl;
  heap.pop_max();
  heap.Print();*/
}

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
#include "../cache.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/";
  if (argc > 1) {
    file_path = argv[1];
  }
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::LoadCaches(file_path, 1, 1, caches);
}

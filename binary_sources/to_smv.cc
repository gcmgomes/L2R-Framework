#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include "../base/dataset.h"

using namespace std;

unsigned Bin(double val, unsigned bin_count) {
  return floor(val * ((double)bin_count-1));
}

bool bla(unsigned f) {
  return f == 8 || f ==  79 || f ==  82 || f ==  130 || f ==  162 || f ==  228;
}

void MakeSpecs(unsigned label_count, unsigned t_n) {
  string ret_val = "SPEC AG(";
  int i = 0;
  char num[200];
  sprintf(num, "EF(t_%u & ", t_n);
  while (i < label_count) {
    int j = i + 1;
    char label_i[200];
    sprintf(label_i, "decider.label = %u) & ", i);
    i++;
    while (j < label_count) {
      char label_j[200];
      sprintf(label_j, "decider.label = %u))", j);
      // (EF(t & decider.label = i) & EF(t & decider.label = j))
      ret_val += "(";
      ret_val += num;
      ret_val += label_i;
      ret_val += num;
      ret_val += label_j;
      if (i != label_count - 1) {
        ret_val += " | ";
      }
      j++;
    }
  }
  cout << endl << ret_val << ')' << endl;
}

void ToSmv(unsigned t_n, ::base::Document& d) {
  string ret_val;
  auto f = d.begin();
  char num[200];
  sprintf(num, "t_%u := (", t_n);
  ret_val = num;
  while (f != d.end()) {
    while (bla(f->first) && rand() % 2) {
      ++f;
    }
    unsigned bin = Bin(f->second, 2);
    sprintf(num, "v_%u.value = %u", f->first, bin);
    ret_val += num;
    ++f;
    if (f != d.end()) {
      ret_val += " & ";
    }
  }
  ret_val += ");";
  cout << ret_val << endl;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input dataset] [label count]" << endl;
    return 0;
  }
  srand(time(NULL));
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  auto q = dataset.begin();
  int i = 0, label_count = 0;
  sscanf(argv[2], "%d", &label_count);
  cout << "\tDEFINE" << endl;
  while (q != dataset.end()) {
    auto d = q->begin();
    while (d != q->end()) {
      cout << "\t\t";
      ToSmv(i, *d);
      i++;
      ++d;
    }
    ++q;
  }
  while (i) {
    MakeSpecs(label_count, --i);
  }
}

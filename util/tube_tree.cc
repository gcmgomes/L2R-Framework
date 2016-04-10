#include <algorithm>
#include <queue>
#include <stack>
#include "tube_tree.h"
using namespace std;
namespace util {

namespace {

struct Comparer {
  bool operator()(const TubeNode* a, const TubeNode* b) {
    return a->log_likelihood() < b->log_likelihood();
  }
};

}  // namespace

void TubeTree::Build(std::vector<double> values) {
  std::sort(values.begin(), values.end());
  tube_input_.reset(new TubeInput(values));
  double lower = values.front(),
         upper = values.back() + tube_input_->minimum_gap();
  int bins = 1;

  root_.reset(new TubeNode(tube_input_.get(), lower, upper));
  root_->FindSplitPoint();

  std::priority_queue<TubeNode*, std::vector<TubeNode*>, Comparer> split_queue;
  split_queue.push(root_.get());
  while (bins < bin_count_) {
    TubeNode* node = NULL;
    while (node == NULL && !split_queue.empty()) {
      node = split_queue.top();
      split_queue.pop();
      if (!node->Split()) {
        if (TubeNode::verbose()) {
          cerr << "Can't split at point " << node->mid() << " #############"
               << endl;
        }
        node = NULL;
      } else {
        bins++;
        if (TubeNode::verbose()) {
          std::cerr << endl
                    << "Transforming bin " << endl
                    << node->ToString() << " in " << endl
                    << node->left()->ToString() << endl
                    << " and " << endl
                    << node->right()->ToString() << endl
                    << endl;
          cerr << " ##################### Found a splittable bin with count = "
               << bins << " at point " << node->mid()
               << " and L = " << node->log_likelihood() << endl;
        }
      }
    }
    if (node == NULL || bins == bin_count_) {
      break;
    }
    if (TubeNode::verbose()) {
      cerr << "Left the loop" << endl;
    }
    if (bins < bin_count_) {
      node->left()->FindSplitPoint();
      node->right()->FindSplitPoint();
      split_queue.push(node->left());
      split_queue.push(node->right());
    }
  }

  SetupDiscretization();
}

unsigned TubeTree::Discretize(double feature_value) const {
  TubeNode* node = root_.get();
  unsigned ret_val = -1;
  while (node != NULL) {
    if (node->left() == NULL && node->right() == NULL) {
      ret_val = node->discretized_value();
      break;
    }
    if (feature_value < node->mid()) {
      node = node->left();
    } else {
      node = node->right();
    }
  }
  return ret_val;
}

void TubeTree::ListUpperIntervalLimits(
    std::vector<double>& upper_split_points) {
  stack<TubeNode*> node_stack;
  node_stack.push(root_.get());
  while (!node_stack.empty()) {
    TubeNode* node = node_stack.top();
    node_stack.pop();
    if (node->left() == NULL) {
      upper_split_points.push_back(node->upper());
    } else {
      node_stack.push(node->right());
      node_stack.push(node->left());
    }
  }
}

void TubeTree::SetupDiscretization() {
  if (TubeNode::verbose()) {
    std::cerr << TubeNode::StringHeader() << endl;
  }
  unsigned discretized_value = 0;
  std::stack<TubeNode*> calls;
  calls.push(root_.get());
  while (!calls.empty()) {
    TubeNode* node = calls.top();
    calls.pop();
    if (node->left() != NULL &&
        node->right() !=
            NULL) {  // By construction, right == NULL iff left == NULL
      calls.push(node->right());
      calls.push(node->left());
    } else {
      node->mutable_discretized_value() = discretized_value++;
      node->mutable_upper() = std::min(node->upper(), tube_input_->values().back());
      if (TubeNode::verbose()) {
        cerr << node->ToString() << endl;
      }
    }
  }
}

}  // namespace util

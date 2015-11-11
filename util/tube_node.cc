#include <algorithm>
#include <cmath>
#include "tube_node.h"

using namespace std;

namespace util {

static double Likelihood(unsigned total, unsigned count, double width,
                         double total_width) {
  double n = total, n_range = count,
         val = n_range *
               log((n_range + (width / total_width)) / (width * (n + 1)));
  if (TubeNode::verbose()) {
    cerr << "Likelihood(N = " << total << ", n = " << count << ", w = " << width
         << ", W = " << total_width << ") = " << val << endl;
  }
  return val;
}

std::pair<double, double> TubeNode::DividedLikelihood(unsigned begin,
                                                      unsigned split,
                                                      unsigned end) const {
  const auto& values = tube_input_->values();
  const auto& counts = tube_input_->counts();
  const auto& sum_prefix = tube_input_->sum_prefix();

  double splitter = values.at(split), lower_split = splitter - EPS,
         upper_split = splitter + EPS;
  unsigned n = sum_prefix.back(), minimum_support = ceil(sqrt(0.1 * (double)n)),
           min_total_left = tube_input_->CountPoints(lower_, lower_split),
           max_total_left = tube_input_->CountPoints(lower_, upper_split),
           total = tube_input_->CountPoints(lower_, upper_);
  double width_left = values.at(split) - lower_,
         width_right = std::min(upper_, values.back()) - values.at(split),
         total_width = values.back() - values.front();
  double ret_val = -100000, split_point = ret_val;

  double left_log_likelihood =
      Likelihood(n, max_total_left, std::min(width_left + EPS, total_width),
                 total_width) +
      Likelihood(n, total - max_total_left, std::max(width_right - EPS, EPS),
                 total_width);
  if (verbose()) {
    cerr << "Left L = " << left_log_likelihood << endl;
  }
  double right_log_likelihood =
      Likelihood(n, min_total_left, std::max(width_left - EPS, EPS),
                 total_width) +
      Likelihood(n, total - min_total_left,
                 std::min(width_right + EPS, total_width), total_width);
  if (verbose()) {
    cerr << "Right L = " << right_log_likelihood << endl;
  }

  double left_min = std::min(width_left + EPS, width_right - EPS);
  if (left_min < MIN_RANGE_SUPPORT) {
    if (verbose()) {
      cerr << "Ignoring left because left_min = " << left_min
           << " || n_left = " << max_total_left
           << " || n_right = " << total - max_total_left
           << " our minimum_support = " << minimum_support
           << " and our MIN_RANGE_SUPPORT = " << MIN_RANGE_SUPPORT << endl;
    }
    left_log_likelihood = ret_val;
  }

  double right_min = std::min(width_left - EPS, width_right + EPS);
  if (right_min < MIN_RANGE_SUPPORT) {
    if (verbose()) {
      cerr << "Ignoring right because right_min = " << right_min
           << " || n_left = " << max_total_left
           << " || n_right = " << total - max_total_left
           << " our minimum_support = " << minimum_support
           << " and our MIN_RANGE_SUPPORT = " << MIN_RANGE_SUPPORT << endl;
    }
    right_log_likelihood = ret_val;
  }

  if (left_log_likelihood >= right_log_likelihood) {
    if (verbose()) {
      cerr << "Case 1 split" << endl;
    }
    split_point = values.at(split) + EPS;
  } else if (left_log_likelihood < right_log_likelihood) {
    if (verbose()) {
      cerr << "Case 2 split" << endl;
    }
    split_point = values.at(split) - EPS;
  }
  if (left_log_likelihood == ret_val && right_log_likelihood == ret_val) {
    if (verbose()) {
      cerr << "Case 3 split with left = " << left_log_likelihood
           << " and right = " << right_log_likelihood << endl;
    }
    split_point = -1;
  }

  double max_likelihood = std::max(left_log_likelihood, right_log_likelihood);

  return std::make_pair(split_point, max_likelihood);
}
using namespace std;

std::pair<double, double> TubeNode::FindSplitPoint() {
  const auto& values = tube_input_->values();
  const auto& counts = tube_input_->counts();
  const auto& sum_prefix = tube_input_->sum_prefix();

  unsigned begin = std::lower_bound(values.cbegin(), values.cend(), lower_) -
                   values.cbegin();
  unsigned end = std::lower_bound(values.cbegin(), values.cend(), upper_) -
                 values.cbegin();
  unsigned n = tube_input_->CountPoints(begin, end - 1);
  unsigned i = begin;
  if (verbose()) {
    cerr << "Going to search from " << lower_ << " located at " << begin
         << " until " << upper_ << " located at " << end << " with a total of "
         << n << " points among them" << endl;
    cerr << "\tOur minimum length support is " << MIN_RANGE_SUPPORT << endl;
  }
  while (i < end + (values.back() == upper_)) {
    double n_left = tube_input_->CountPoints(begin, i),
           n_right = tube_input_->CountPoints(i, end - 1);
    if (n_left >= 1 || n_right >= 1) {  // Gotta make sure we have the minimum
                                        // support on each divison.
      if (verbose()) {
        cerr << endl
             << "Splitting around " << i << " = " << values[i]
             << " with n_left = " << n_left << " and n_right = " << n_right
             << endl;
      }
      std::pair<double, double> current = DividedLikelihood(begin, i, end);
      double split = current.first, log_likelihood = current.second;
      if (verbose()) {
        cerr << "Checking if split at " << split
             << " and L = " << log_likelihood << " is better then the current"
             << endl;
      }
      if (split > lower_) {  // Gotta make sure we have a valid split.
        if (log_likelihood_ < log_likelihood) {
          mid_ = split;
          log_likelihood_ = log_likelihood;
        }
      }
    } else {
      if (verbose()) {
        cerr << "Didn't check anything because n_left = " << n_left
             << " and n_right = " << n_right << endl;
      }
    }
    i++;
  }
  double width = std::min(upper_, values.back()) - lower_;
  if (verbose()) {
    cerr << "discout = ";
  }
  double discount =
      Likelihood(sum_prefix.back(), n, width, values.back() - values.front());
  if (verbose()) {
    cerr << "Splitting is defined by mid_ = " << mid_
         << " and L = " << log_likelihood_ << " and discount = " << discount
         << " and gain = " << log_likelihood_ - discount << endl;
  }
  this->log_likelihood_ -= discount;
  if (verbose()) {
    std::cerr << "Returning (" << mid_ << ", " << log_likelihood_ << ") "
              << endl;
  }
  return std::make_pair(mid_, log_likelihood_);
}

bool TubeNode::Split() {
  if (lower_ >= mid_) {
    return false;
  }
  left_.reset(new TubeNode(tube_input_, lower_, mid_));
  right_.reset(new TubeNode(tube_input_, mid_, upper_));
  return true;
}

std::string TubeNode::StringHeader() {
  return std::string(
      "#|    Bin                   |     %    ||  Instances \t |\t\t%\t\t "
      "| "
      "     Density | Probability | Loglk");
}

std::string TubeNode::ToString() const {
  const auto& values = tube_input_->values();
  const auto& counts = tube_input_->counts();
  const auto& sum_prefix = tube_input_->sum_prefix();
  char num[2048];
  double w = std::min(upper_, values.back()) - lower_,
         W = values.back() - values.front(), N = sum_prefix.back();
  unsigned begin = std::lower_bound(values.cbegin(), values.cend(), lower_) -
                   values.cbegin();
  unsigned end = std::lower_bound(values.cbegin(), values.cend(), upper_) -
                 values.cbegin();
  unsigned sum_end = sum_prefix.at(end - 1), sum_begin = sum_prefix.at(begin);
  unsigned n = sum_end - sum_begin;
  double w_percentage = w / W;
  n += counts.at(begin);
  double n_percentage = (double)100 * n / N,
         density = (n + (w / W)) / (w * (N + 1)), probability = (double)n / N;
  double likelihood = n * log(density);
  sprintf(num,
          "#%u: #| [%lf, %lf) |  %lf\% || \t %u \t |  %.5lf\% \t | %u.0 \t |D "
          "%.5lf |P "
          "%.5lf |L "
          "%.5lf",
          discretized_value_, lower_, std::min(upper_, values.back()),
          w_percentage, n, n_percentage, n, density, probability, likelihood);
  return string(num);
}

}  // namespace util

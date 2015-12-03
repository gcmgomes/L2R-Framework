#include <algorithm>
#include <cmath>
#include "tube_node.h"

namespace util {

static double Likelihood(unsigned total, unsigned count, double width,
                         double total_width) {
  if (width <= 0.0) {
    return 0;
  }
  double n = total, n_range = count,
         val = n_range *
               log((n_range + (width / total_width)) / (width * (n + 1)));
  if (TubeNode::verbose()) {
    std::cerr << "Likelihood(N = " << total << ", n = " << count
              << ", w = " << width << ", W = " << total_width << ") = " << val
              << std::endl;
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

  if (verbose()) {
    std::cerr << "we have that n = " << n
              << ", min_total_left = " << min_total_left
              << " and max_total_right " << max_total_left << std::endl;
  }

  double left_log_likelihood =
      Likelihood(n, max_total_left, std::min(width_left + EPS, total_width),
                 total_width) +
      Likelihood(n, total - max_total_left, std::max(width_right - EPS, EPS),
                 total_width);
  if (verbose()) {
    std::cerr << "Left L = " << left_log_likelihood << std::endl;
  }
  double right_log_likelihood =
      Likelihood(n, min_total_left, std::max(width_left - EPS, EPS),
                 total_width) +
      Likelihood(n, total - min_total_left,
                 std::min(width_right + EPS, total_width), total_width);
  if (verbose()) {
    std::cerr << "Right L = " << right_log_likelihood << std::endl;
  }

  double left_min = std::min(width_left + EPS, width_right - EPS);
  if (left_min < MIN_RANGE_SUPPORT) {
    if (verbose()) {
      std::cerr << "Ignoring left because left_min = " << left_min
                << " || n_left = " << max_total_left
                << " || n_right = " << total - max_total_left
                << " our minimum_support = " << minimum_support
                << " and our MIN_RANGE_SUPPORT = " << MIN_RANGE_SUPPORT
                << std::endl;
    }
    left_log_likelihood = ret_val;
  }

  double right_min = std::min(width_left - EPS, width_right + EPS);
  if (right_min < MIN_RANGE_SUPPORT) {
    if (verbose()) {
      std::cerr << "Ignoring right because right_min = " << right_min
                << " || n_left = " << max_total_left
                << " || n_right = " << total - max_total_left
                << " our minimum_support = " << minimum_support
                << " and our MIN_RANGE_SUPPORT = " << MIN_RANGE_SUPPORT
                << std::endl;
    }
    right_log_likelihood = ret_val;
  }

  if (left_log_likelihood >= right_log_likelihood) {
    if (verbose()) {
      std::cerr << "Case 1 split" << std::endl;
    }
    split_point = values.at(split) + EPS;
  } else if (left_log_likelihood < right_log_likelihood) {
    if (verbose()) {
      std::cerr << "Case 2 split" << std::endl;
    }
    split_point = values.at(split) - EPS;
  }
  if (left_log_likelihood == ret_val && right_log_likelihood == ret_val) {
    if (verbose()) {
      std::cerr << "Case 3 split with left = " << left_log_likelihood
                << " and right = " << right_log_likelihood << std::endl;
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
    std::cerr << "Going to search from " << lower_ << " located at " << begin
              << " until " << upper_ << " located at " << end
              << " with a total of " << n << " points among them" << std::endl;
    std::cerr << "\tOur minimum length support is " << MIN_RANGE_SUPPORT
              << std::endl;
  }
  while (i < end + (values.back() == upper_)) {
    double n_left = tube_input_->CountPoints(begin, i),
           n_right = tube_input_->CountPoints(i, end - 1);
    if (n_left >= 1 || n_right >= 1) {  // Gotta make sure we have the minimum
                                        // support on each divison.
      if (verbose()) {
        std::cerr << std::endl
                  << "Splitting around " << i << " = " << values[i]
                  << " with n_left = " << n_left << " and n_right = " << n_right
                  << std::endl;
      }
      std::pair<double, double> current = DividedLikelihood(begin, i, end);
      double split = current.first, log_likelihood = current.second;
      if (verbose()) {
        std::cerr << "Checking if split at " << split
                  << " and L = " << log_likelihood
                  << " is better then the current" << std::endl;
      }
      if (split > lower_) {  // Gotta make sure we have a valid split.
        if (log_likelihood_ < log_likelihood) {
          mid_ = split;
          log_likelihood_ = log_likelihood;
        }
      }
    } else {
      if (verbose()) {
        std::cerr << "Didn't check anything because n_left = " << n_left
                  << " and n_right = " << n_right << std::endl;
      }
    }
    i++;
  }
  double width = std::min(upper_, values.back()) - lower_;
  if (verbose()) {
    std::cerr << "discout = ";
  }
  double discount =
      Likelihood(sum_prefix.back(), n, width, values.back() - values.front());
  if (verbose()) {
    std::cerr << "Splitting is defined by mid_ = " << mid_
              << " and L = " << log_likelihood_
              << " and discount = " << discount
              << " and gain = " << log_likelihood_ - discount << std::endl;
  }
  this->log_likelihood_ -= discount;
  if (verbose()) {
    std::cerr << "Returning (" << mid_ << ", " << log_likelihood_ << ") "
              << std::endl;
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
  double w_percentage = (W <= 0.0) ? 100 : 100 * w / W;
  n += counts.at(begin);
  double n_percentage = (double)100 * n / N,
         density = (W <= 0.0) ? 0.0 : (n + (w / W)) / (w * (N + 1)),
         probability = (double)n / N;
  double likelihood = n * log(density);
  if (density > 0.0) {
    sprintf(num,
            "#%u: #| [%.8lf, %.8lf) |  %.8lf\% || \t %u \t |  %.8lf\% \t | "
            "%u.0 \t |D "
            "%.8lf |P "
            "%.8lf |L "
            "%.8lf",
            discretized_value_, lower_, std::min(upper_, values.back()),
            w_percentage, n, n_percentage, n, density, probability, likelihood);
  } else {
    sprintf(num,
            "#%u: #| [%.8lf, %.8lf) |  %.8lf\% || \t %u \t |  %.8lf\% \t | "
            "%u.0 \t |D "
            "INF \t|P "
            "%.8lf    |L "
            "INF",
            discretized_value_, lower_, std::min(upper_, values.back()),
            w_percentage, n, n_percentage, n, probability);
  }
  return string(num);
}

}  // namespace util

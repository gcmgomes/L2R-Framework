package(default_visibility = ["//visibility:public"])

cc_library(
  name = "ranker_helper",
  srcs = ["ranker_helper.cc"],
  hdrs = ["ranker_helper.h"],
  deps = ["//base:dataset",
          "//bayes/branch_and_bound:cache",
          "//bayes/branch_and_bound:graph",
          "//bayes/branch_and_bound:variable",
          "//bayes/branch_and_bound/inference:ranker",
          "//bayes/branch_and_bound/inference:cp_table"],
  linkopts = ["-lm"],
)

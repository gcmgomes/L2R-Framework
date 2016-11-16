package(default_visibility = ["//visibility:public"])

cc_library(
  name = "grasp",
  hdrs = ["grasp.h"],
  srcs = ["grasp.cc"],
  deps = ["//bayes/branch_and_bound:graph",
          "//bayes/branch_and_bound:variable",
          "//bayes/branch_and_bound:inverted_index",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:backtrack_local_search",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:greedy_local_search"],
)


cc_library(
  name = "pathrelinking",
  hdrs = ["pathrelinking.h"],
  srcs = ["pathrelinking.cc"],
  deps = ["//bayes/branch_and_bound/heuristic:grasp",
          "//bayes/branch_and_bound:graph",
          "//bayes/branch_and_bound:variable",
          "//bayes/branch_and_bound:inverted_index",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:backtrack_local_search",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:greedy_local_search"],
)

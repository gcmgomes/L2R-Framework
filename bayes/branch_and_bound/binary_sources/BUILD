package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "cache_builder",
  srcs = ["cache_builder.cc"],
  deps = ["//bayes/branch_and_bound:cache_builder"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "per_query_cache_builder",
  srcs = ["per_query_cache_builder.cc"],
  deps = ["//bayes/branch_and_bound:cache_builder"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "dataset_builder",
  srcs = ["dataset_builder.cc"],
  deps = ["//bayes/branch_and_bound:instance"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "branch_and_bound",
  srcs = ["branch_and_bound.cc"],
  deps = ["//bayes/branch_and_bound:branch_and_bound"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "summarize_caches",
  srcs = ["summarize_caches.cc"],
  deps = ["//bayes/branch_and_bound:variable"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "best_global_graph",
  srcs = ["best_global_graph.cc"],
  deps = ["//bayes/branch_and_bound:branch_and_bound"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "grasp",
  srcs = ["grasp.cc"],
  deps = ["//bayes/branch_and_bound/heuristic:grasp",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:backtrack_local_search",
          "//bayes/branch_and_bound/external_priority_queue/heuristics:greedy_local_search",
          "//bayes/branch_and_bound:branch_and_bound"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "pathrelinking",
  srcs = ["pathrelinking.cc"],
  deps = ["//bayes/branch_and_bound/heuristic:pathrelinking",
          "//bayes/branch_and_bound:branch_and_bound"],
)

cc_binary(
  name = "graph_reader",
  srcs = ["graph_reader.cc"],
  deps = ["//bayes/branch_and_bound/heuristic:pathrelinking",
          "//bayes/branch_and_bound:graph",
          "//bayes/branch_and_bound:branch_and_bound"],
)

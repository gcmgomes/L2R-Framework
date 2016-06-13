package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "cache_building",
  srcs = ["cache_building.cc"],
  deps = ["//bayes/branch_and_bound:variable"],
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

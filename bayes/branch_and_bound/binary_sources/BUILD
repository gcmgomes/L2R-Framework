package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "cache_building",
  srcs = ["cache_building.cc"],
  deps = ["//bayes/branch_and_bound:indexed_variable"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "branch_and_bound",
  srcs = ["branch_and_bound.cc"],
  deps = ["//bayes/branch_and_bound:branch_and_bound"],
  linkopts = ["-lm"],
)

package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "external_branch_and_bound",
  srcs = ["external_branch_and_bound.cc"],
  deps = ["//bayes/branch_and_bound/external_priority_queue:external_branch_and_bound"],
  linkopts = ["-lm"],
)

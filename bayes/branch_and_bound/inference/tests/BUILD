package(default_visibility = ["//visibility:public"])

cc_binary(
  name =  "cp_table",
  srcs = ["cp_table.cc"],
  deps = ["//bayes/branch_and_bound/inference:cp_table",
          "//bayes/branch_and_bound:variable"],
)

cc_binary(
  name =  "ranker",
  srcs = ["ranker.cc"],
  deps = ["//bayes/branch_and_bound/inference:ranker"],
)

cc_binary(
  name =  "sampler",
  srcs = ["sampler.cc"],
  deps = ["//bayes/branch_and_bound/inference:sampler"],
)

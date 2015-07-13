package(default_visibility = ["//visibility:public"])

cc_library(
  name = "normalizer",
  srcs = ["normalizer.cc"],
  hdrs = ["normalizer.h"],
  deps = ["//base:document",
          "//base:query"],
)

cc_binary(
  name = "normalizer_test",
  srcs = ["normalizer_test.cc"],
  deps = ["//base:dataset",
          ":normalizer"],
)

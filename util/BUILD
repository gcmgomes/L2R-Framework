package(default_visibility = ["//visibility:public"])

cc_library(
  name = "statistics",
  srcs = ["statistics.cc"],
  hdrs = ["statistics.h"],
  deps = ["//base:document",
          "//base:query"],
)

cc_library(
  name = "normalizer",
  srcs = ["normalizer.cc"],
  hdrs = ["normalizer.h"],
  deps = ["//base:document",
          "//base:query",
          ":statistics"],
)

cc_binary(
  name = "normalizer_test",
  srcs = ["normalizer_test.cc"],
  deps = ["//base:dataset",
          ":normalizer"],
)

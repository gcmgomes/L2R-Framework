package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "to_smv",
  srcs = ["to_smv.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_binary(
  name = "treater",
  srcs = ["treater.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_binary(
  name = "fold_splitter",
  srcs = ["fold_splitter.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_binary(
  name = "smoter",
  srcs = ["smoter.cc"],
  deps = ["//knn:knn",
          "//knn:smote",
          "//base:dataset",
          "//base:document",
          "//base:query",
          "//euclid:vector_operations"],
)

cc_binary(
  name = "correlation_matrix",
  srcs = ["correlation_matrix.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query",
          "//util:statistics"],
)

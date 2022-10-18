load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_custom_op_library")

tf_custom_op_library(
    # add_one_kernel.cc  add_one_kernel.cu.cc  add_one_kernel.h
    name = "add_one_kernel.so",
    srcs = ["add_one_kernel.h", "add_one_kernel.cc"],
    gpu_srcs = ["add_one_kernel.cu.cc", "add_one_kernel.h"],
)

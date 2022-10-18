#ifndef ADD_ONE_KERNEL_H_
#define ADD_ONE_KERNEL_H_

#include <unsupported/Eigen/CXX11/Tensor>

// Parent template
template <typename Device, typename T>
struct KernelFunctor {
  void operator()(const Device& d, const int size, const T* in, T* out);
};

#ifdef GOOGLE_CUDA
// GPU partial specialization template declaration
template <typename T>
struct KernelFunctor<Eigen::GpuDevice, T> {
  void operator()(const Eigen::GpuDevice& d, const int size, const T* in, T* out);
};
#endif

#endif ADD_ONE_KERNEL_H_
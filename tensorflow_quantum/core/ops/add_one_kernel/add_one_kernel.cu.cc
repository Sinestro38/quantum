#ifdef GOOGLE_CUDA
#define EIGEN_USE_GPU

#include "add_one_kernel.h"

#include "stdio.h"
#include "tensorflow/core/util/gpu_kernel_helper.h"

using GPUDevice = Eigen::GpuDevice;

template <typename T>
__global__ void add_one(const int size, const T* in, T* out) {
  int start_idx = blockIdx.x * blockDim.x + threadIdx.x;
  for (int i = start_idx; i < size; i += blockDim.x * gridDim.x) {
    out[i] = in[i] + 1;
    printf("Adding one in the GPU... \n");
  }
}

// GPU kernel template launcher definition
template <typename T>
void KernelFunctor<GPUDevice, T>::operator()(const GPUDevice& d, const int size,
                                             const T* in, T* out) {
  // launch cuda kernel to add one foreach
  int block_count = 1024;
  int thread_per_block = 20;
  add_one<T><<<block_count, thread_per_block>>>(size, in, out);
}

// Explicitly instantiate functor templates
// -- you need this because while the .cc kernel_example TU is getting compiled,
// it won't be able to instantiate the GPU partial specialized template
// (generate code) like it can with the CPU template because the GPU template is
// in a different TU that was already compiled (code has already been generated)
template struct KernelFunctor<GPUDevice, int>;
template struct KernelFunctor<GPUDevice, float>;

#endif  // GOOGLE_CUDA
#include "add_one_kernel.h"

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;

using CPUDevice = Eigen::ThreadPoolDevice;
using GPUDevice = Eigen::GpuDevice;

REGISTER_OP("PavanAddOne")
    .Attr("T: numbertype")
    .Input("input: T")
    .Output("output: T")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    });

// CPU partial specialization template declaration&definition
template <typename T>
struct KernelFunctor<CPUDevice, T> {
  void operator()(const CPUDevice& d, const int size, const T* in, T* out) {
    for (int i = 0; i < size; i++) {
      out[i] = in[i] + 1;
    }
  }
};

template <typename Device, typename T>
class MyAddOneOp : public OpKernel {
 public:
  explicit MyAddOneOp(OpKernelConstruction* context) : OpKernel(context) {}

  void Compute(OpKernelContext* context) {
    // fetch inputs
    const Tensor& input_tensor = context->input(0);
    const T* input_arr = input_tensor.flat<T>().data();

    // allocate outputs
    Tensor* output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    T* output_arr = output_tensor->flat<T>().data();

    // do the computation
    auto kernel_compute = KernelFunctor<Device, T>();
    kernel_compute(context->eigen_device<Device>(),
                   static_cast<int>(input_tensor.NumElements()), input_arr,
                   output_arr);
  }
};

// Register CPU kernels
REGISTER_KERNEL_BUILDER(
    Name("PavanAddOne").Device(DEVICE_CPU).TypeConstraint<int>("T"),
    MyAddOneOp<CPUDevice, int>);
REGISTER_KERNEL_BUILDER(
    Name("PavanAddOne").Device(DEVICE_CPU).TypeConstraint<float>("T"),
    MyAddOneOp<CPUDevice, float>);

// following is not violating ODR since definition contains same contents
extern template struct KernelFunctor<GPUDevice, int>;    // optional
extern template struct KernelFunctor<GPUDevice, float>;  // optional
// Register GPU kernels using a macro
#define REGISTER_GPU(T)                                              \
  REGISTER_KERNEL_BUILDER(                                           \
      Name("PavanAddOne").Device(DEVICE_GPU).TypeConstraint<T>("T"), \
      MyAddOneOp<GPUDevice, T>)
REGISTER_GPU(int);
REGISTER_GPU(float);

// Longer way of defining them
// REGISTER_KERNEL_BUILDER(
//     Name("PavanAddOne").Device(DEVICE_GPU).TypeConstraint<int>("T"),
//     MyAddOneOp<GPUDevice, int>);
// REGISTER_KERNEL_BUILDER(
//     Name("PavanAddOne").Device(DEVICE_GPU).TypeConstraint<float>("T"),
//     MyAddOneOp<GPUDevice, float>);

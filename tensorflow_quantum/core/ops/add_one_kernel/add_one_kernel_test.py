import tensorflow as tf

op_path = r"bazel-bin/tensorflow/core/user_ops/add_one_kernel/add_one_kernel.so"
class ZeroOutTest(tf.test.TestCase):
  def testZeroOut(self):
    kernel_module = tf.load_op_library(op_path)
    kernel = kernel_module.pavan_add_one
    with self.test_session():
      result = kernel([1, 2, 3, 4, 5])
      print("RESULT: ", result.numpy())
      self.assertAllEqual(result.numpy(), [2, 3, 4, 5, 6])

if __name__ == "__main__":
  tf.test.main()
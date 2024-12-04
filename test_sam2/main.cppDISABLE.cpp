//Example explained here: https://onnxruntime.ai/docs/tutorials/mnist_cpp.html
//Check model info here: https://github.com/onnx/models/tree/main/validated/vision/classification/mnist
//MNIST’s input is a {1,1,28,28} shaped float tensor, which is basically a 28x28 floating point grayscale image (0.0 = background, 1.0 = foreground).
//MNIST’s output is a simple {1,10} float tensor that holds the likelihood weights per number. The number with the highest value is the model’s best guess.
//The MNIST structure uses std::max_element to do this and stores it in result_:

#include <stdio.h>  
#include "mnist.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

void print_MNIST_digit(cv::Mat img) 
{
  //opencv Mat row,col -> image y,x (the intuitive way)
  for (unsigned i = 0; i < 28; i++) {
    for (unsigned j = 0; j < 28; j++) {
      if (img.at<uchar>(i, j)>30)
        printf("# ");
      else
        printf("  ");
    }
    printf("\n");
  }
  printf("\n");
}

int main()
{
  printf("Hello world\n");

  //1) Prepare the ONNX model (file mnist.onnx harcoded in mnist.h)
  std::unique_ptr<MNIST> mnist_;
  mnist_ = std::make_unique<MNIST>();

  //2) Read input image
  std::string image_path = "img3.jpg";
  cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
  /* (debug show the image)
  if(img.empty())
  {
      printf("Could not read the image\n");
      return 1;
  }
  printf("Take a look at the small image in the top left corner and press a key\n");
  cv::imshow("Display window", img);
  int k = cv::waitKey(0); // Wait for a keystroke in the window
  */
  print_MNIST_digit(img); //(debug) show the image as ASCII
  
  //3) Convert the opencv Mat image to ONNX tensor
  //In typdef MSNIT in mnist.h: 
  //  std::array<float, width_ * height_> input_image_{};
  float* output = mnist_->input_image_.data();
  std::fill(mnist_->input_image_.begin(), mnist_->input_image_.end(), 0.f);
  for (unsigned y = 0; y < MNIST::height_; y++) {
    for (unsigned x = 0; x < MNIST::width_; x++) {
      output[y * MNIST::height_ + x] = img.at<uchar>(y, x)/(float)255;
    }
  }

  //Run the model
  mnist_->Run();
  
  //Results
  auto least = *std::min_element(mnist_->results_.begin(), mnist_->results_.end());
  auto greatest = mnist_->results_[mnist_->result_];
  auto range = greatest - least;

  for (unsigned i = 0; i < 10; i++) {
    int y = 16 * i;
    float result = mnist_->results_[i];
    printf("%2d: %d.%02d\n", i, int(result), abs(int(result * 100) % 100));
  }
  return 0;
}

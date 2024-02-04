//
// Created by irving on 24-1-25.
//

#include <rm_dgl_proc.h>
#define GREEN_THRESHOLD 30 // 绿色阈值（0-255，越高筛选越绿）
#define C_THRESHOLD 60 // 圆阈值（0-正无穷，越低筛选越圆）
// 图像宽高（测试模式不需要）
#define WIDTH 100
#define HEIGHT 100
#if TEST
#define IMAGE_PATH "/home/irving/图片/images.jpeg" // 测试图片路径
#endif

namespace RmDglProc {
/// 实现绿色过滤
void Processor::filterGreenColor(const unsigned char *image, int width,
                                 int height, int green_threshold) {

#if TEST
  image_filtered_ = new unsigned char[width * height];
#endif

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const unsigned char *pixel = &image[(y * width + x) * 3];
      unsigned char *pixel_filtered = &image_filtered_[(y * width + x)];
      if (pixel[1] > pixel[0] + green_threshold &&
          pixel[1] > pixel[2] + green_threshold) { // G > R and G > B
        // 保留绿色像素，将其设置为白色
        *pixel_filtered = 255;
      } else {
        // 非绿色像素，将其设置为黑色
        *pixel_filtered = 0;
      }
    }
  }
}

#if TEST
/// 使用OpenCV读取图像
void Processor::loadImageData(const std::string &imagePath) {
  cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
  if (img.empty()) {
    std::cerr << "Error: Unable to load image." << std::endl;
    return;
  }

  // 设置图像尺寸
  width_ = img.cols;
  height_ = img.rows;

  // 分配内存给imageData_
  imageData_ = new unsigned char[width_ * height_ * 3];

  // 将OpenCV图像数据复制到imageData_
  if (img.isContinuous()) {
    std::memcpy(imageData_, img.data, width_ * height_ * 3);
  } else {
    for (int i = 0; i < height_; ++i) {
      std::memcpy(imageData_ + i * width_ * 3, img.ptr(i), width_ * 3);
    }
  }
}

/// 使用OpenCV显示图像
void Processor::showFilteredResult() {
  if (image_filtered_ == nullptr) {
    std::cerr << "Error: Filtered image data is not available." << std::endl;
    return;
  }

  // 创建一个OpenCV的Mat对象来包装数据
  //        cv::Mat filteredImg(height_, width_, CV_8UC1, image_filtered_);
  cv::Mat filteredImg = cv::Mat(height_, width_, CV_8UC1);

  // 存入轮廓至Mat
  std::cout << "轮廓数" << contours_.count() << std::endl;
  for (unsigned int i = 0; i < contours_.count(); ++i) {
    for (unsigned int j = 0; j < contours_[i].count() - 1; ++j) {
      filteredImg.at<uchar>(contours_[i][j].y, contours_[i][j].x) = 255;
    }
  }

  // 输出检测出圆信息，并画出圆心与半径
  std::cout << "圆数" << circles_.count() << std::endl;
  for (int i = 0; i < circles_.size; ++i) {
    filteredImg.at<uchar>(circles_[i].center.y, circles_[i].center.x) = 255;
    cv::line(filteredImg, cv::Point(circles_[i].center.x, circles_[i].center.y),
             cv::Point(circles_[i].center.x + circles_[i].radius,
                       circles_[i].center.y),
             cv::Scalar(255), 1);
    std::cout << "圆" << i + 1 << " x:" << circles_[i].center.x << " y："
              << circles_[i].center.y << " 半径：" << circles_[i].radius
              << std::endl;
  }

  // 显示图像
  cv::imshow("Filtered Image", filteredImg);
  cv::waitKey(0);
}
#endif

/// 总处理函数
void Processor::process() {
  filterGreenColor(imageData_, width_, height_, green_threshold_);
#if TEST
  contoursDetector_ = new ContoursDetector::ContoursDetector(width_, height_);
#endif
  contours_.clear();
  circles_.clear();
  contours_ = contoursDetector_->detectContours(image_filtered_);
  circles_ = contoursDetector_->detectCircles(contours_, c_threshold_);
}

} // namespace RmDglProc

int main() {
#if TEST
  // 获取开始时间点
  auto start = std::chrono::high_resolution_clock::now();

  RmDglProc::Processor processor(WIDTH, HEIGHT, GREEN_THRESHOLD, C_THRESHOLD);
  processor.loadImageData(IMAGE_PATH);
  processor.process();
  processor.showFilteredResult(); // 显示过滤后的结果

  // 获取结束时间点
  auto end = std::chrono::high_resolution_clock::now();
  // 计算运行时间(微秒)
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  // 输出运行时间
  std::cout << "Time taken to execute: " << duration << " microseconds" << std::endl;
#else
  RmDglProc::Processor processor(640, 640, GREEN_THRESHOLD, C_THRESHOLD);
  processor.process();
#endif
}

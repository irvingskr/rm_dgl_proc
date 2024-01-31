//
// Created by irving on 24-1-25.
//

#ifndef RM_DGL_PROC_RM_DGL_PROC_H
#define RM_DGL_PROC_RM_DGL_PROC_H
#include <contours_detector.h>
#define TEST 1 // 是否进入OpenCV测试模式
#if TEST
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#endif
#endif // RM_DGL_PROC_RM_DGL_PROC_H

namespace RmDglProc {

class Processor {
public:
  Processor(int width, int height, int green_threshold, int c_threshold)
      : width_(width), height_(height), green_threshold_(green_threshold),
        c_threshold_(c_threshold) {
    imageData_ = new unsigned char[width * height];
    image_filtered_ = new unsigned char[width * height];
    contoursDetector_ = new ContoursDetector::ContoursDetector(width, height);
  };
  ~Processor() {
    delete[] imageData_;
    delete[] image_filtered_;
    delete contoursDetector_;
  };

#if TEST
  /// Opencv读取/显示图像到数组
  void loadImageData(const std::string &imagePath);
  void showFilteredResult();
#endif

  /// 颜色过滤
  void filterGreenColor(const unsigned char *image, int width, int height,
                        int green_threshold);

  /// 总处理
  void process();

  RmStd::Vector<ContoursDetector::Circle> circles_;

private:
  /// 图像
  // 原图
  unsigned char *imageData_;
  // 滤波后的图
  unsigned char *image_filtered_;
  int width_;
  int height_;
  int green_threshold_;

  /// 轮廓
  int c_threshold_;
  ContoursDetector::ContoursDetector *contoursDetector_;
  RmStd::Vector<RmStd::Vector<ContoursDetector::Point>> contours_;
};

} // namespace RmDglProc
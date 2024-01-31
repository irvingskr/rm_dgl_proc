//
// Created by irving on 24-1-28.
//

#ifndef RM_DGL_PROC_CONTOURS_DETECTOR_H
#define RM_DGL_PROC_CONTOURS_DETECTOR_H
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406
#include <rm_std.h>
#endif // RM_DGL_PROC_CONTOURS_DETECTOR_H

namespace ContoursDetector {
    using namespace RmStd;
/// 定义一个Point结构体来保存点的信息
struct Point {
  int x, y;
};
/// 定义一个Circle结构体来保存圆的信息
struct Circle {
  Point center;
  int radius;
};

class ContoursDetector {
public:
  ContoursDetector(int width, int height)
      : width_(width), height_(height),
        edgeImage_(new unsigned char[width * height]()){};
  ~ContoursDetector() { delete[] edgeImage_; }

  /// 检测边缘（输出二值图）
  void detectEdges(const unsigned char *image) {
    int gx, gy, sum;

    // 遍历图像中的每个像素，只保存边缘
    for (int y = 1; y < height_ - 1; y++) {
      for (int x = 1; x < width_ - 1; x++) {
        // 应用Sobel算子，计算x方向和y方向的梯度
        gx = -image[(y - 1) * width_ + x - 1] +
             image[(y - 1) * width_ + x + 1] - 2 * image[y * width_ + x - 1] +
             2 * image[y * width_ + x + 1] - image[(y + 1) * width_ + x - 1] +
             image[(y + 1) * width_ + x + 1];

        gy = -image[(y - 1) * width_ + x - 1] -
             2 * image[(y - 1) * width_ + x] - image[(y - 1) * width_ + x + 1] +
             image[(y + 1) * width_ + x - 1] + 2 * image[(y + 1) * width_ + x] +
             image[(y + 1) * width_ + x + 1];

        // 计算梯度幅度
        sum = abs(gx) + abs(gy);

        // 如果梯度幅度超过阈值，则在edgeImage_中标记为边缘
        if (sum > 1000) { // 阈值可以调整
          edgeImage_[y * width_ + x] = 255;
        } else {
          edgeImage_[y * width_ + x] = 0;
        }
      }
    }
  };

  /// 检测轮廓（输出vector）
  Vector<Vector<Point>>
  detectContours(const unsigned char *image) {
    // Todo:将局部变量改为类成员变量，并写清空vector操作
    Vector<Vector<Point>> contours;

    // 边缘检测
    // 假设已经有了一个二维边缘图像，edgeImage[y][x] = 255/0
    detectEdges(image);

    // 用于标记已访问的边缘点
    auto *visited = new unsigned char[width_ * height_];

    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        if (edgeImage_[y * width_ + x] && !visited[y * width_ + x]) {
          Vector<Point> contour;
          // 跟踪轮廓
          trackContour(x, y, visited, contour);
          contours.push_back(contour);
        }
      }
    }

    delete[] visited;
    return contours;
  };

  /// 从边缘点递归跟踪轮廓
  void trackContour(int x, int y, unsigned char *visited,
                    Vector<Point> &contour) {
    // 检查边界条件
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
      return;
    }

    // 检查是否为边缘点且未被访问
    if (!edgeImage_[y * width_ + x] || visited[y * width_ + x]) {
      return;
    }

    // 标记当前点为已访问并添加到轮廓中
    visited[y * width_ + x] = true;
    contour.push_back(Point{x, y});

    // 递归地检查周围的点（上、下、左、右）
    trackContour(x, y - 1, visited, contour); // 上
    trackContour(x, y + 1, visited, contour); // 下
    trackContour(x - 1, y, visited, contour); // 左
    trackContour(x + 1, y, visited, contour); // 右
  };

  /// 圆形轮廓判断
  static Vector<Circle>
  detectCircles(Vector<Vector<Point>> &contours,
                int c_threshold) {
    // Todo:将局部变量改为类成员变量，并写清空vector操作
    Vector<Circle> circles;

    for (int i = 0; i < contours.size; ++i) {
      Vector<Point> &contour = contours[i];
      double area = calculateArea(contour);
      double perimeter = calculatePerimeter(contour);

      double circularity = (perimeter * perimeter) / (4 * M_PI * area);
      // 近似程度测试
      //                std::cout<<"轮廓"<<i+1<<"近似程度"<<circularity<<std::endl;

      if (abs(circularity - 1.0) < c_threshold) {
        Circle circle = fitCircleToContour(contour);
        circles.push_back(circle);
      }
    }

    return circles;
  };

  /// 计算轮廓的面积
  static double calculateArea(Vector<Point> &contour) {
    double area = 0.0;
    int n = contour.size;

    for (int i = 0; i < n - 1; ++i) {
      area += contour[i].x * contour[i + 1].y;
      area -= contour[i].y * contour[i + 1].x;
    }
    area += contour[n - 1].x * contour[0].y;
    area -= contour[n - 1].y * contour[0].x;

    return abs(area / 2.0);
  }

  /// 计算轮廓的周长
  static double calculatePerimeter(Vector<Point> &contour) {
    int n = contour.size;

    // 计算周长，即所有相邻点对之间距离的总和
    double perimeter = 0.0;
    for (int i = 0; i < n - 1; ++i) {
      perimeter += sqrt(pow(contour[i + 1].x - contour[i].x, 2) +
                             pow(contour[i + 1].y - contour[i].y, 2));
    }
    // 加上最后一个点到第一个点的距离
    perimeter += sqrt(pow(contour[0].x - contour[n - 1].x, 2) +
                           pow(contour[0].y - contour[n - 1].y, 2));

    return perimeter;
  };

  /// 计算圆心和半径
  static Circle fitCircleToContour(Vector<Point> &contour) {
    int n = contour.size;

    // 计算平均位置
    double avgX = 0.0, avgY = 0.0;
    for (int i = 0; i < n; ++i) {
      avgX += contour[i].x;
      avgY += contour[i].y;
    }
    avgX /= n;
    avgY /= n;

    // 计算半径
    double radius = 0.0;
    for (int i = 0; i < n; ++i) {
      radius += sqrt(pow(contour[i].x - avgX, 2) + pow(contour[i].y - avgY, 2));
    }
    radius /= n;

    return Circle{Point{static_cast<int>(avgX), static_cast<int>(avgY)},
                  static_cast<int>(radius)};
  }

private:
  unsigned char *edgeImage_;
  int width_, height_;
};

} // namespace ContoursDetector
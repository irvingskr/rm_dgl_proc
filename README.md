# rm_dgl_proc

## Description

### [rm_vector.h](include%2Frm_vector.h)

- 声明和实现了`vector`类，实现vector的操作函数
- 实现数学库中的标准函数 `std::abs`, `std::sqrt`, 和 `std::pow` 

### [rm_dgl_proc.h](include%2Frm_dgl_proc.h)

- 声明并实现了`ContoursDetector`类，实现`ContoursDetector`类的函数
  
  - 轮廓提取函数`detectContours`
  - 圆提取函数`detectCircles`
### [rm_dgl_proc.h](include%2Frm_dgl_proc.h) 

- 声明Processor类，和一些成员函数
  - `OpenCV`测试输入输出函数`loadImageData showFilteredResult`
  - 颜色滤波函数`filterGreenColor`
  - 总处理函数`process`
- 宏定义OpenCV调试模式
```c++
  #define TEST // 是否进入OpenCV测试模式
  #if TEST
  #include <iostream>
  #include <opencv4/opencv2/opencv.hpp>
  #endif
```

### [rm_dgl_proc.cpp](src%2Frm_dgl_proc.cpp)

- 实现`Processor`类的函数
- 主函数（包括测试功能）
- 开头定义宏进行调参
```c++
  #define GREEN_THRESHOLD 20 // 绿色阈值（0-255，越高筛选越绿（条件越苛刻））
  #define C_THRESHOLD 60 // 圆阈值（0-正无穷，越低筛选越圆（条件越苛刻））
  // 图像宽高（测试模式不需要）
  #define WIDTH 640 
  #define HEIGHT 640
  #if TEST
  #define IMAGE_PATH "/home/irving/图片/images.jpeg" // 测试图片路径
  #endif
```
# Camera 模块说明

## 简介

Camera 模块主要用于处理对相机的操作，如开关相机、取图、设置参数等。

## 使用方法

在本项目中，用户一般无需直接操作相机类（已封装进 VideoSource 模块）。如果需要直接操作相机，请手动构造相机对象，再调用公共接口进行操作。

### 构造相机对象

包含`camera-base.h`，并调用`camera::CreateCamera(type_name)`获取对象指针。

```c++
#include "camera-base/camera-base.h"
int main() {
  // 指定类型名，当前支持 DHCamera (大恒 Galaxy) 和 HikCamera (海康 MVS)
  std::string type_name = "HikCamera";  // 还可填写 "DHCamera"
  // 使用传统指针，需要手动释放
  Camera* camera_legacy_ptr = camera::CreateCamera(type_name);
  delete camera_legacy_ptr
  // 使用智能指针
  std::unique_ptr<Camera> camera_unique_ptr = std::make_unique<Camera>(camera::CreateCamera(type_name));
}
```

### 打开相机并开始取图

调用`OpenCamera(serial_number, config_file)`打开相机（返回是否打开成功），成功后调用`StartStream()`打开取图流。

打开取图流后，预先定义一个帧结构体`frame`调用`GetFrame(frame)`，将帧信息写入`frame`中。

```c++
#include "camera-base/camera-base.h"
int main() {
  // 构造指定类型的相机对象
  std::unique_ptr<Camera> camera = std::make_unique<Camera>(camera::CreateCamera("HikCamera"));
  // 打开相机，需要指定序列号和配置文件（配置文件可以为空）
  if (!camera->OpenCamera("00D27551311", "../config/cameras/MV-CA016-10UC_00D27551311.mfs"))
    return 0;  // 打开相机失败
  // 开始取图
  if (!camera->StartStream())
    return 0;  // 无法打开取图流
  // 取一张图并显示
  Frame frame;
  camera->GetFrame(frame);
  cv::imshow("Frame", frame.image);
  cv::waitKey(0);
  // 如果使用传统指针，请在结束时手动释放相机
  // delete camera;
  return 0;
}
```

### 关闭取图流与相机

相机指针释放时会自动执行关闭取图流和相机的操作。调用`StopStream()`手动关闭取图流，然后调用`CloseCamera()`
手动关闭相机。关闭取图流后方可调整部分相机参数，关闭相机后方可打开其他相机，而不用构造新的相机对象。

```c++
#include "camera-base/camera-base.h"
int main() {
  std::unique_ptr<Camera> camera = std::make_unique<Camera>(camera::CreateCamera("HikCamera"));
  if (!camera->OpenCamera("00D27551311", "../config/cameras/MV-CA016-10UC_00D27551311.mfs")) return 0;
  if (!camera->StartStream()) return 0;
  if (!StopStream()) return 0;  // 手动关闭取图流
  if (!CloseCamera()) return 0;  // 手动关闭相机
  // 重新打开相机，可以是其他序列号和配置
  if (!camera->OpenCamera("00D27551311", "")) return 0;
  return 0;  // 自动释放
}
```

### 其他操作

以上内容仅包含了相机的基本开关操作。若要使用更多功能，请阅读下方技术细节后，查看`camera-base.h`的函数说明。

## 技术细节

### 抽象接口的封装

由于队内使用多个品牌的相机，Camera 模块提供了操作不同相机 API 的公共接口，并且将属于单个相机的系统资源（句柄、守护线程等）封装到类中，为将来可能同时操作多个相机作准备。

### 具体实现的整合与调用方式

Camera 模块使用自动注册工厂模式向下构建（关于该模式的具体信息和实现，参见 Common 模块说明）。仅根据给定的
API 标识（字符串）就可以直接构造针对具体 API 的 Camera 派生类，并且取得 Camera 公共接口类的指针，可以达到完全隐藏派生类的目的。

### 触发源、缓冲区与回调机制

由于实际取图操作为相机触发（向相机 API 注册回调函数，取图时调用），Camera
类维护一个存取帧结构体的缓冲区，具体类型为一个自动丢弃旧数据的带锁循环队列（参见 Common
模块说明），相机调用回调函数时将帧结构体存入缓冲区中，外界调用取图时从将缓冲区中取出帧结构体。

然而，使用相机触发回调会导致实际取图的时刻无法进行其他操作（如读取云台姿态等），因此 Camera 类采用注册取图时回调函数的方案。Camera
类接受外界任意数量的回调函数，在即将将帧结构体放入缓冲区时依次调用每个回调函数，允许外界对帧结构体信息进行修改（同时扩展帧结构体的数据内容），实现帧信息的同步。

### 自动重连

一旦相机开启，Camera 类将维护一个守护线程。当相机连接意外中断时，守护线程将关闭相机、释放系统资源并（间隔 1
秒）反复尝试重新连接相机，直到连接成功为止。关闭相机时将向守护线程发送停止信号，停止线程。
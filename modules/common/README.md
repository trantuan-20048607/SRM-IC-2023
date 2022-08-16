# Common 模块说明

Common 模块为通用数据结构和功能的集合，不同文件互不关联。

## armor

Armor 是用于描述装甲板的类。由于在创建装甲板时即完成各类坐标系的转换和求解，因此需要在 Controller 初始化完成后才能创建
Armor 对象。

```c++
#include "common/armor.h"
int controller::hero::HeroController::Run() {
  //...
  std::array<cv::Point2f, 4> points;  // 指定角点
  coordinate::EAngle ea_imu;  // 指定云台姿态
  Armor::ArmorSize size = Armor::ArmorSize::SMALL;  // 指定装甲板大小
  Armor armor{points, coord_solver_, ea_imu, size};  // 构造装甲板
  //...
}
```

Armor 类提供了读取转换后各个坐标的函数，参阅`armor.h`中的函数说明（`attr-reader`的用法见下节）。

## attr-reader

`attr_reader`是来自 Ruby 语言的语法糖，用于生成读取（不能修改）类私有成员变量的函数。在 C++
中，函数传参分为引用传递和值传递等方法，`attr_reader`提供了引用传递版本`attr_reader_ref`和值传递版本`attr_reader_val`。

```c++
#define attr_reader_val(_var, _func) inline auto _func() const { return _var; }
#define attr_reader_ref(_var, _func) inline auto &&_func() const { return _var; }
```

其中，`_var`是要访问的变量名，`_func`是访问该变量的函数名。

```c++
#include "common/attr-reader.h"
class Foo {
 public:
  attr_reader_val(bar, Bar)  // 使用值传递，调用 Bar() 获取变量值
  attr_reader_ref(str, Str)  // 使用引用传递，调用 Str() 获取变量值
 private:
  int bar;
  std::string str;
};
```

## buffer

`Buffer`主要用于多线程中作数据缓冲区，使用带锁循环队列实现。队列满后使用新数据覆盖旧数据以保证实时性。

`Buffer`类包含两个模板参数，分别为数据类型和队列容量。为了提升性能，`Buffer`类入队操作使用完美转发方式实现，请注意使用右值引用移动参数。

```c++
#include "common/buffer.h"
int main() {
  Buffer<int, 16> buf;
  buf.Push(1);  // 放入数据，对于变量，请使用 std::move() 并注意移动后的处理
  int e;
  buf.Pop(e);  // 取出数据，返回队列是否为空，将覆盖变量 e 的值
  std::cout << e << std::endl;
}
```

## factory

使用模板实现了自动注册工厂模式。相比传统工厂模式，省略了工厂创建对象时产生的多分支结构，提高了类继承关系和子类选择的稳定性。

### 实现步骤

1. 通过模板创建属于基类的工厂（接受注册信息基类的指针）和注册信息基类
2. 定义注册信息子类，并通过模板保存要构造的子类信息，在构造注册信息子类时直接将自己注册到基类，并保留一个构造子类的函数
3. 在子类中声明静态的注册子类成员，将子类自身作为注册子类模板的参数
4. 定义（构造）注册子类成员，同时为要构造的子类指定一个字符串标识符，此时工厂中已经保存了构造子类的函数指针
5. 在外界通过工厂调用给定标识符所对应的子类，返回动态分配内存得到的对象指针

这样，外界将无需知道子类的真实名称（甚至无需引用子类的头文件），便可以直接得到基类指针形式返回的子类实体，并依赖公共接口进行操作。

### 使用方法

作者已经将实现步骤中的第 1、2 步封装在宏函数中，在根命名空间中使用`enable_factory(_namespace, _base_class)`
构建，其中`_namespace`表示基类所处的命名空间，`_base_class`为基类名称。

```c++
#include "common/factory.h"
enable_factory(camera, Camera)  // 可以在定义基类之前使用此封装
namespace camera {
// 定义基类
class Camera {
  //...
};
}
```

在子类中声明一个私有的静态成员，类型为`Registry<_sub_class>`，其中`_sub_class`为子类名称，并在子类对应的源文件中初始化。

```c++
#include "common/factory.h"
namespace camera::dh {
// 子类
class DHCamera final : public Camera {
 public:
  //...
 private:
  //...
  static Registry<DHCamera> registry;  // 子类注册信息
};
}
// 在源文件中声明注册信息，其中 "DHCamera" 为子类标识符
camera::Registry<camera::dh::DHCamera> camera::dh::DHCamera::registry("DHCamera");
//...
```

最后，在外界调用`namespace_name::CreateBaseClass(type_name)`函数以获得基类指针形式的子类实体，其中`namespace_name`
为基类所属命名空间的名称，`BaseClass`为基类名，`type_name`为子类标识符。

```c++
#include "camera-base/camera-base.h"
int main() {
  // 创建子类对象
  auto camera = std::make_unique<Camera>(camera::CreateCamera("DHCamera"));
}
```

请注意，此模式仅适用于构造函数没有自变量的基类。

## frame

`Frame`是存储帧数据的结构体，包含图像、姿态等需要实时获取的信息。`Frame`结构体在相机取图操作被触发时生成，经过视频源回调函数处理后进入缓冲区，再由
Controller 模块取出并使用，最终销毁。

```c++
struct Frame {
  cv::Mat image;  // 图像
  ReceivePacket receive_packet;  // 接收到的云台姿态等数据
  uint64_t time_stamp;  // 时间戳，用于修正延迟
};
```

可以向`Frame`结构体中添加内容，然后通过视频源回调函数进行其他处理，参见 Camera 模块的回调机制部分。

## packet

`Packet`是存储串口收发数据的结构体，分为`SendPacket`和`ReceivePacket`两部分。

`Packet`数据包的结构是固定的，由下位机电控决定，并由 Serial 模块处理收发，参见 Serial 模块说明。
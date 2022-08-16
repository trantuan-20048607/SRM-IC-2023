# CLIArgParser 模块使用说明

## 简介

CLIArgParser 模块用于处理命令行调用参数，决定是否开关部分功能、是否显示界面等。

## 使用方法

本模块基于 GFlags 实现功能，并将读到的参数封装于独立的结构`cli_argv`中，可全局访问。

### 指定命令行参数

#### 指定参数

运行时添加`-var=val`以指定参数（多个参数以空格区分），其中`var`为变量名，`val`为变量值。

```shell
./srm-ic-2023 -string_var="" -bool_var=false
```

#### 查看帮助

运行时只指定`-help`参数即可查看帮助。

```shell
./srm-ic-2023 -help
```

### 添加命令行参数

#### 修改 cli-arg-parser.h

在类`cli::CliArgParser`添加`private`变量，支持`bool`、`string`、`int`、`double`等类型。

在类`cli::CliArgParser`中使用`attr_reader_ref(_var, _func)`或`attr_reader_val(_var, _func)`
添加可供外界访问（只读）的变量`_val`并分配对应的函数`_func`。其中`attr_reader_ref`
生成的函数向外传出常量引用，`attr_reader_val`生成的函数向外传出值拷贝。

```c++
namespace cli {
class CliArgParser final {
 public:
  //...
  attr_reader_ref(string_var, StringVar)  // 调用 StringVar() 读取值，对于字符串使用引用传递
  attr_reader_val(bool_var, BoolVar)  // 调用 BoolVar() 读取值，对于布尔值使用值传递
  // ...
 private:
  // ...
  std::string string_var;  // 字符串类型
  bool bool_var;  // 布尔类型
};
```

#### 修改 cli-arg-parser.cpp

在文件最上方增加一行`DEFINE_type(var, default, description)`定义一个名为`var`的有效命令行参数（将`type`
换成变量类型），并设定默认值为`default`。`description`为变量描述，将在帮助页面中显示。

在`cli::CliArgParser::Parse`函数返回之前增加一行`var = FLAGS_var;`，将左边的`var`换成成员变量的名称，并将右边的`var`
换成上方定义的命令行参数变量名。

```c++
#include "cli-arg-parser.h"
DEFINE_string(string_var, "", "string var");
DEFINE_string(bool_var, "", "bool var");
//...
void cli::CliArgParser::Parse(int argc, char **argv) {
  //...
  string_var = FLAGS_string_var;
  bool_var = FLAGS_bool_var;
  //...
}
```

### 读取命令行参数

引入`cli-arg-parser.h`，对全局变量`cli_argv`调用对应函数即可。

```c++
#include "cli-arg-parser.h"
int main() {
  // cli_argv.Parse(argc, argv);
  std::cout << cli_argv.string_var << std::endl;
}
```
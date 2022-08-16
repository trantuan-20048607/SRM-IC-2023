#ifndef SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_
#define SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_

#include <string>
#include "common/attr-reader.h"

namespace cli {
class CliArgParser final {
 public:
  static CliArgParser &Instance();

  attr_reader_ref(controller_type_, ControllerType)
  attr_reader_ref(video_source_type_, VideoSourceType)
  attr_reader_val(record_, Record)
  attr_reader_val(serial_, Serial)
  attr_reader_val(ui_, UI)

  /**
   * @brief 解析命令行参数
   * @param argc 命令行参数 ARGC，从主函数中获取
   * @param argv 命令行参数 ARGV，从主函数中获取
   */
  void Parse(int argc, char **argv);

 private:
  CliArgParser() = default;
  ~CliArgParser() = default;

  std::string controller_type_, video_source_type_;
  bool record_{}, serial_{}, ui_{};
};
}

extern cli::CliArgParser &cli_argv;  ///< 记录命令行参数的全局变量

#endif  // SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_

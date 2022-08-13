#ifndef SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_
#define SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_

#include <string>
#include "common/attr-reader.h"

namespace cli {
class CliArgParser {
 public:
  static CliArgParser &Instance();

  attr_reader_ref(controller_type_, ControllerType)
  attr_reader_ref(video_source_type_, VideoSourceType)
  attr_reader_val(record_, Record)
  attr_reader_val(serial_, Serial)
  void Parse(int argc, char **argv);

 private:
  CliArgParser() = default;
  ~CliArgParser() = default;

  std::string controller_type_, video_source_type_;
  bool record_{}, serial_{};
};
}

extern cli::CliArgParser &cli_argv;

#endif  // SRM_IC_2023_MODULES_CLI_ARG_PARSER_CLI_ARG_PARSER_H_

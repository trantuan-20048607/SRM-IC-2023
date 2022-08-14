#include <csignal>
#include <glog/logging.h>
#include "cli-arg-parser/cli-arg-parser.h"
#include "controller-base/controller-base.h"

bool controller::Controller::exit_signal_ = false;

void SignalHandler(int signal) {
  if (signal == SIGINT) {
    LOG(WARNING) << "Caught interrupt signal. Attempting to exit...";
    controller::Controller::exit_signal_ = true;
  } else if (signal == SIGTERM) {
    LOG(WARNING) << "Caught terminate signal. Attempting to exit...";
    controller::Controller::exit_signal_ = true;
  }
}

int main(int argc, char **argv) {
  cli_argv.Parse(argc, argv);
  auto controller = controller::CreateController(cli_argv.ControllerType());
  if (!controller) return -1;
  if (!controller->Initialize()) return 1;
  std::signal(SIGINT, &SignalHandler);
  std::signal(SIGTERM, &SignalHandler);
  int ret = controller->Run();
  delete controller;
  google::ShutdownGoogleLogging();
  return ret;
}

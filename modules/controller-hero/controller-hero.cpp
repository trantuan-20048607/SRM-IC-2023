#include "controller-hero.h"

controller::Registry<controller::hero::HeroController> controller::hero::HeroController::registry_("hero");

bool controller::hero::HeroController::Initialize() {
  return controller::Controller::Initialize("hero");
}

int controller::hero::HeroController::Run() {
  return 0;
}

#ifndef SRM_IC_2023_MODULES_CONTROLLER_HERO_CONTROLLER_HERO_H_
#define SRM_IC_2023_MODULES_CONTROLLER_HERO_CONTROLLER_HERO_H_

#include "controller-base/controller-base.h"

namespace controller::hero {
class HeroController final : public Controller {
 public:
  bool Initialize() final;
  int Run() final;

 private:
  static Registry<HeroController> registry_;
};
}

#endif  // SRM_IC_2023_MODULES_CONTROLLER_HERO_CONTROLLER_HERO_H_

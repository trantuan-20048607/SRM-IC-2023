#ifndef SRM_IC_2023_MODULES_COMMON_FACTORY_H_
#define SRM_IC_2023_MODULES_COMMON_FACTORY_H_

#include <unordered_map>

namespace factory {
template<class B>
class RegistryBase {
 public:
  virtual B *Create() = 0;

 protected:
  RegistryBase() = default;
  virtual ~RegistryBase() = default;
};

template<class B>
class Factory final {
 public:
  inline static Factory &Instance() {
    static Factory factory;
    return factory;
  }

  inline void Register(const std::string &type_name, RegistryBase<B> *registry) {
    registry_[type_name] = registry;
  }

  inline B *Create(const std::string &type_name) {
    return registry_.find(type_name) != registry_.end() ? registry_[type_name]->Create() : nullptr;
  }

 private:
  Factory() = default;
  ~Factory() = default;

  std::unordered_map<std::string, RegistryBase<B> *> registry_;
};

template<class B, class S>
class RegistrySub final : public RegistryBase<B> {
 public:
  explicit RegistrySub(const std::string &type_name) {
    Factory<B>::Instance().Register(type_name, this);
  }

  inline B *Create() final { return new S(); }
};
}

#endif  // SRM_IC_2023_MODULES_COMMON_FACTORY_H_

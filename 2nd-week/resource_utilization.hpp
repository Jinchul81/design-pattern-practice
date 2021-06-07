#include <string_view>

#include "generator.hpp"

template <typename T>
struct IResourceUtilization {
  virtual ~IResourceUtilization() = default;
  virtual T get() = 0;
  virtual std::string_view unit() const = 0;
};

template <typename T = int>
class CpuUtilization : public IResourceUtilization<T> {
  RandomNumericGenerator<T> rand;

public:
  CpuUtilization()
    : rand(0, 100) {
  }
  virtual ~CpuUtilization() = default;

  virtual T get() override {
    return rand.get();
  }
  virtual std::string_view unit() const override {
    return "%";
  }
};

template <typename T = double>
class DiskUtilization : public IResourceUtilization<T> {
  RandomFloatingGenerator<> rand;

public:
  DiskUtilization()
    : rand(1, 1e3) {
  }
  virtual ~DiskUtilization() = default;

  virtual T get() override {
    return rand.get();
  }
  virtual std::string_view unit() const override {
    return "TB";
  }
};

template <typename T = double>
class MemoryUtilization : public IResourceUtilization<T> {
  RandomFloatingGenerator<> rand;

public:
  MemoryUtilization()
    : rand(2 << 3, 2 << 10) {
  }
  virtual ~MemoryUtilization() = default;

  virtual T get() {
    return rand.get();
  }
  virtual std::string_view unit() const override {
    return "GB";
  }
};

template <typename T = double>
class NetworkUtilization : public IResourceUtilization<T> {
  RandomFloatingGenerator<> rand;

public:
  NetworkUtilization()
    : rand(1e2, 1e3) {
  }
  virtual ~NetworkUtilization() = default;

  virtual T get() {
    return rand.get();
  }
  virtual std::string_view unit() const override {
    return "GB";
  }
};

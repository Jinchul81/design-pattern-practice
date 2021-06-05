#include <iostream>

#include "resource_utilization.hpp"

struct IMessage {
  ~IMessage() = default;
  virtual std::ostream& operator<<(std::ostream& o) = 0;
};

std::ostream& operator<<(std::ostream& o, IMessage& msg) {
  return msg << o;
}

class CpuUtilizationMessage : public IMessage {
  CpuUtilization<> cpu_usage;

public:
  virtual ~CpuUtilizationMessage() = default;

  virtual std::ostream& operator<<(std::ostream& o) override {
    o << "CPU utilization: " << cpu_usage.get()
      << " " << cpu_usage.unit() << std::endl;
    return o;
  };
};

class DiskUtilizationMessage : public IMessage {
  DiskUtilization<> disk_usage;

public:
  virtual ~DiskUtilizationMessage() = default;

  virtual std::ostream& operator<<(std::ostream& o) override {
    o << "Disk utilization: " << disk_usage.get()
      << " " << disk_usage.unit() << std::endl;
    return o;
  };
};

class MemoryUtilizationMessage : public IMessage {
  MemoryUtilization<> memory_usage;

public:
  virtual ~MemoryUtilizationMessage() = default;

  virtual std::ostream& operator<<(std::ostream& o) override {
    o << "Memory utilization: " << memory_usage.get()
      << " " << memory_usage.unit() << std::endl;
    return o;
  };
};

class NetworkUtilizationMessage : public IMessage {
  NetworkUtilization<> network_usage;

public:
  virtual ~NetworkUtilizationMessage() = default;

  virtual std::ostream& operator<<(std::ostream& o) override {
    o << "Network utilization: " << network_usage.get()
      << " " << network_usage.unit() << std::endl;
    return o;
  };
};

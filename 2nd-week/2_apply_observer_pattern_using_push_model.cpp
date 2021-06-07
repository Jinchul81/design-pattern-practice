#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <sstream>
#include <iostream>

#include "message.hpp"
#include "file_handler.hpp"

using namespace std;

struct ILogger {
  virtual ~ILogger() = default;

  virtual void write(string_view msg) = 0;
};

class ConsoleLogger : public ILogger {
public:
  virtual ~ConsoleLogger() = default;

  virtual void write(string_view msg) override {
    cout << msg;
  }
};

class FileLogger : public ILogger {
  FileHandler file_handler;

public:
  FileLogger(string_view out_filename)
    : file_handler(out_filename) {
  }
  virtual ~FileLogger() = default;

  virtual void write(string_view msg) override {
    auto& out = file_handler.get();
    out << msg;
  }
};

template <typename Observer>
struct IObserverable {
  virtual ~IObserverable() = default;

  virtual void subscribe(Observer& obj) = 0;
  virtual void unsubscribe(Observer& obj) = 0;
  virtual void notify() = 0;
};

template <typename T>
class ResourceManager {
  unordered_map<string_view, T*> dict;

public:
  void add(string_view name, T& resource) {
    if (dict.count(name))
      throw invalid_argument(
        "The name \""s + name.data() + "\" exists");
    dict[name] = &resource;
  }

  void erase(string_view name) {
    if (!dict.count(name))
      throw invalid_argument(
        "The name \""s + name.data() + "\" does not exist");
    dict.erase(name);
  }

  auto begin() {
    return dict.begin();
  }

  auto end() {
    return dict.end();
  }

  auto cbegin() {
    return dict.cbegin();
  }

  auto cend() {
    return dict.cend();
  }
};

template <typename Observer = ILogger>
class LoggingService : public IObserverable<Observer> {
  ResourceManager<IMessage>& resource_manager;
  unordered_set<Observer*> observers;
  mutex mtx;

public:
  LoggingService(ResourceManager<IMessage>& resource_manager)
    : resource_manager(resource_manager) {
  }

  virtual ~LoggingService() = default;

  virtual void subscribe(Observer& obj) override {
    scoped_lock<mutex>{mtx};
    observers.insert(&obj);
  }

  virtual void unsubscribe(Observer& obj) override {
    scoped_lock<mutex>{mtx};
    if (observers.count(&obj))
      observers.erase(&obj);
  }

  virtual void notify() override {
    scoped_lock<mutex>{mtx};
    auto itr = resource_manager.begin();
    for (; itr != resource_manager.end(); ++itr) {
      ostringstream oss;
      oss << *(itr->second);
      for (auto& observer:observers) {
        observer->write(oss.str());
      }
    }
  }
};

void test(string_view out_filename, size_t count) {
  CpuUtilizationMessage cpu_usage;
  DiskUtilizationMessage disk_usage;
  MemoryUtilizationMessage memory_usage;
  NetworkUtilizationMessage network_usage;

  ResourceManager<IMessage> resource_manager;
  resource_manager.add("CPU"sv, cpu_usage);
  resource_manager.add("Disk"sv, disk_usage);
  resource_manager.add("Memory"sv, memory_usage);
  resource_manager.add("Network"sv, network_usage);

  ConsoleLogger console_logger;
  FileLogger file_logger(out_filename);

  LoggingService<> logging_service(resource_manager);
  logging_service.subscribe(console_logger);
  logging_service.subscribe(file_logger);
  for (size_t i = 0; i < count; ++i)
    logging_service.notify();
}

int main() {
  test("resource.log"sv, 1000);

  return 0;
}

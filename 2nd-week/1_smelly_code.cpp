#include <sstream>

#include "resource_utilization.hpp"
#include "file_handler.hpp"

using namespace std;

class ResourceMonitor {
  FileHandler file_handler;
  CpuUtilization<> cpu_usage;
  DiskUtilization<> disk_usage;
  MemoryUtilization<> memory_usage;
  NetworkUtilization<> network_usage;

  void console_logger(string_view msg) {
    cout << msg;
  }

  void file_logger(string_view msg) {
    auto& out = file_handler.get();
    out << msg;
  }

  void flush(string_view msg) {
    console_logger(msg);
    file_logger(msg);
  }

  void update_cpu_usage() {
      ostringstream oss;
      oss << "CPU utilization: " << cpu_usage.get()
          << " " << cpu_usage.unit() << endl;
      flush(oss.str());
  }

  void update_disk_usage() {
      ostringstream oss;
      oss << "Disk utilization: " << disk_usage.get()
          << " " << disk_usage.unit() << endl;
      flush(oss.str());
  }

  void update_memory_usage() {
      ostringstream oss;
      oss << "Memory utilization: " << memory_usage.get()
          << " " << memory_usage.unit() << endl;
      flush(oss.str());
  }

  void update_network_usage() {
      ostringstream oss;
      oss << "Network utilization: " << network_usage.get()
          << " " << network_usage.unit() << endl;
      flush(oss.str());
  }

public:
  ResourceMonitor(string_view out_filename)
    : file_handler(out_filename) {
  }
  virtual ~ResourceMonitor() = default;

  void update() {
    update_cpu_usage();
    update_disk_usage();
    update_memory_usage();
    update_network_usage();
  }
};

void test(string_view out_filename, size_t count) {
  ResourceMonitor monitor(out_filename);
  for (size_t i = 0; i < count; ++i) {
    monitor.update();
  }
}

int main() {
  test("resource.log"sv, 1000);

  return 0;
}

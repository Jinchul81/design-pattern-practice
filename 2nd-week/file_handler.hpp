#include <fstream>
#include <string_view>

// RAII for file descriptor
class FileHandler {
  std::ofstream out_file;

public:
  FileHandler(std::string_view out_filename)
    : out_file(out_filename.data()) {
  }

  virtual ~FileHandler() {
    if (is_open()) out_file.close();
  }

  bool is_open() const {
    return out_file.is_open();
  }

  std::ofstream& get() {
    if (!is_open()) {
      throw std::runtime_error(
        "Unexpected error: the stream is already closed.");
    }
    return out_file;
  }
};

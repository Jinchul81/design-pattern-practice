#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

struct IProcess {
  virtual void open() = 0;
  virtual void read() = 0;
  virtual void parse() = 0;
  virtual void analyze() = 0;
  virtual void report() = 0;
  virtual void close() = 0;

  virtual void execute() final {
    open();
    read();
    parse();
    analyze();
    report();
    close();
  }

  virtual ~IProcess() = default;
};

class IDocument : public IProcess {
  string filename;

protected:
  void print_message(string_view type, string_view msg) {
    ostringstream oss;
    oss << "[" << type << "] " << msg << ": '" << filename << "'";
    cout << oss.str() << endl;
  }

public:
  IDocument(string_view filename)
    : filename(filename) {
  }
  IDocument() = delete;
  virtual ~IDocument() = default;
};
using IDocumentSPtr = shared_ptr<IDocument>;

class DocFile : public IDocument {
  static constexpr string_view type = "doc";

public:
  DocFile(string_view filename)
    : IDocument(filename) {
  }
  virtual ~DocFile() = default;

  virtual void open() noexcept override final {
    print_message(type, "The file has been opened");
  }

  virtual void read() noexcept override final {
    print_message(type, "The file has been read");
  }

  virtual void parse() noexcept override final {
    print_message(type, "The file has been parsed");
  }

  virtual void analyze() noexcept override final {
    print_message(type, "The file has been analyzed");
  }

  virtual void report() noexcept override final {
    print_message(type, "The file has been reported");
  }

  virtual void close() noexcept override final {
    print_message(type, "The file has been closed");
  }
};

class CsvFile : public IDocument {
  static constexpr string_view type = "csv";

public:
  CsvFile(string_view filename)
    : IDocument(filename) {
  }
  virtual ~CsvFile() = default;

  virtual void open() noexcept override final {
    print_message(type, "The file has been opened");
  }

  virtual void read() noexcept override final {
    print_message(type, "The file has been read");
  }

  virtual void parse() noexcept override final {
    print_message(type, "The file has been parsed");
  }

  virtual void analyze() noexcept override final {
    print_message(type, "The file has been analyzed");
  }

  virtual void report() noexcept override final {
    print_message(type, "The file has been reported");
  }

  virtual void close() noexcept override final {
    print_message(type, "The file has been closed");
  }
};

class PdfFile : public IDocument {
  static constexpr string_view type = "pdf";

public:
  PdfFile(string_view filename)
    : IDocument(filename) {
  }
  virtual ~PdfFile() = default;

  virtual void open() noexcept override final {
    print_message(type, "The file has been opened");
  }

  virtual void read() noexcept override final {
    print_message(type, "The file has been read");
  }

  virtual void parse() noexcept override final {
    print_message(type, "The file has been parsed");
  }

  virtual void analyze() noexcept override final {
    print_message(type, "The file has been analyzed");
  }

  virtual void report() noexcept override final {
    print_message(type, "The file has been reported");
  }

  virtual void close() noexcept override final {
    print_message(type, "The file has been closed");
  }
};

class HwpFile : public IDocument {
  static constexpr string_view type = "hwp";

public:
  HwpFile(string_view filename)
    : IDocument(filename) {
  }
  virtual ~HwpFile() = default;

  virtual void open() noexcept override final {
    print_message(type, "The file has been opened");
  }

  virtual void read() noexcept override final {
    print_message(type, "The file has been read");
  }

  virtual void parse() noexcept override final {
    print_message(type, "The file has been parsed");
  }

  virtual void analyze() noexcept override final {
    print_message(type, "The file has been analyzed");
  }

  virtual void report() noexcept override final {
    print_message(type, "The file has been reported");
  }

  virtual void close() noexcept override final {
    print_message(type, "The file has been closed");
  }
};

void test() {
  vector<IDocumentSPtr> docs {
    make_shared<DocFile>("test.doc"),
    make_shared<CsvFile>("test.csv"),
    make_shared<PdfFile>("test.pdf"),
    make_shared<HwpFile>("test.hwp")
  };
  for_each(docs.begin(), docs.end(), [](auto& e) {
    e->execute();
  });
}

int main() {
  test();
  return 0;
}

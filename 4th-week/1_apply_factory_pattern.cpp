#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct IApp {
  virtual string_view description() const = 0;
};
using IAppSPtr = shared_ptr<IApp>;

struct HealthApp : public IApp {
  virtual string_view description() const override {
    return "health app";
  };
};

struct GameApp : public IApp {
  virtual string_view description() const override {
    return "game app";
  };
};

struct MusicApp : public IApp {
  virtual string_view description() const override {
    return "music app";
  };
};

struct AppFactory {
  static IAppSPtr createHealthApp() {
    return make_shared<HealthApp>();
  }
  static IAppSPtr createGameApp() {
    return make_shared<GameApp>();
  }
  static IAppSPtr createMusicApp() {
    return make_shared<MusicApp>();
  }
};

// -------------------------------------------

struct IOS {
  virtual string_view description() const = 0;
};
using IOSSPtr = shared_ptr<IOS>;

struct AppleIOS7 : public IOS {
  virtual string_view description() const override {
    return "Apple IOS 7";
  }
};

struct AppleIOS8 : public IOS {
  virtual string_view description() const override {
    return "Apple IOS 8";
  }
};

struct GoogleAndroid10 : public IOS {
  virtual string_view description() const override {
    return "Google Android 10";
  }
};

struct GoogleAndroid11 : public IOS {
  virtual string_view description() const override {
    return "Google Android 11";
  }
};

struct OSFactory {
  static IOSSPtr createAppleIOS7() {
    return make_shared<AppleIOS7>();
  }
  static IOSSPtr createAppleIOS8() {
    return make_shared<AppleIOS8>();
  }
  static IOSSPtr createGoogleAndroid10() {
    return make_shared<GoogleAndroid10>();
  }
  static IOSSPtr createGoogleAndroid11() {
    return make_shared<GoogleAndroid11>();
  }
};

// -------------------------------------------

class TestDevice {
  IOSSPtr os;
  string_view name;

  ostream& gen(ostream& o) const {
    if (!os) throw runtime_error("os has not been set.");
    o << name << " (" << os->description() << ")";
    return o;
  }

protected:
  explicit TestDevice(string_view name)
    : name(name) {
  }

public:
  void set_os(IOSSPtr os) {
    this->os = os;
  }

  string description() const {
    ostringstream oss;
    gen(oss);
    return oss.str();
  }
};
using TestDeviceSPtr = shared_ptr<TestDevice>;

class GalaxyS20 : public TestDevice {
  static constexpr auto name = "Galaxy s20"sv;

public:
  GalaxyS20()
    : TestDevice(name) {
  }
};

class GalaxyS21 : public TestDevice {
  static constexpr auto name = "Galaxy s21"sv;

public:
  GalaxyS21()
    : TestDevice(name) {
  }
};

class IPhone7 : public TestDevice {
  static constexpr auto name = "IPhone 7"sv;

public:
  IPhone7()
    : TestDevice(name) {
  }
};

class IPhone12 : public TestDevice {
  static constexpr auto name = "IPhone 12"sv;

public:
  IPhone12()
    : TestDevice(name) {
  }
};

struct TestDeviceFactory {
  static TestDeviceSPtr createGalaxyS20() {
    return make_shared<GalaxyS20>();
  }
  static TestDeviceSPtr createGalaxyS21() {
    return make_shared<GalaxyS21>();
  }
  static TestDeviceSPtr createIPhone7() {
    return make_shared<IPhone7>();
  }
  static TestDeviceSPtr createIPhone12() {
    return make_shared<IPhone12>();
  }
};

// -------------------------------------------

class ITestPlatform {
protected:
  TestDeviceSPtr device;
  vector<IOSSPtr> os_list;

  virtual void build(IAppSPtr app) const = 0;
  virtual void test(IAppSPtr app) const = 0;
  virtual void deploy(IAppSPtr app) const = 0;
  virtual void report(IAppSPtr app) const = 0;

public:
  ITestPlatform(TestDeviceSPtr device, vector<IOSSPtr>& os_list)
    : device(device), os_list(os_list.begin(), os_list.end()) {
  }

  virtual void run(IAppSPtr app) final {
    if (!app) throw invalid_argument("Application has not been sent.");
    for_each(os_list.begin(), os_list.end(), [&](auto os) {
      device->set_os(os);
      build(app);
      test(app);
      deploy(app);
      report(app);
    });
  };
};
using ITestPlatformSPtr = shared_ptr<ITestPlatform>;

class TestPlatformForGalaxyS20 : public ITestPlatform {
protected:
  virtual void build(IAppSPtr app) const override {
    cout << "[Build]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void test(IAppSPtr app) const override {
    cout << "[Test]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void deploy(IAppSPtr app) const override {
    cout << "[Deploy]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void report(IAppSPtr app) const override {
    cout << "[Report]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }

public:
  TestPlatformForGalaxyS20(vector<IOSSPtr> os_list)
    : ITestPlatform(TestDeviceFactory::createGalaxyS20(), os_list) {
  }
};

class TestPlatformForGalaxyS21 : public ITestPlatform {
protected:
  virtual void build(IAppSPtr app) const override {
    cout << "[Build]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void test(IAppSPtr app) const override {
    cout << "[Test]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void deploy(IAppSPtr app) const override {
    cout << "[Deploy]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void report(IAppSPtr app) const override {
    cout << "[Report]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }

public:
  TestPlatformForGalaxyS21(vector<IOSSPtr> os_list)
    : ITestPlatform(TestDeviceFactory::createGalaxyS21(), os_list) {
  }
};

class TestPlatformForIPhone7 : public ITestPlatform {
protected:
  virtual void build(IAppSPtr app) const override {
    cout << "[Build]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void test(IAppSPtr app) const override {
    cout << "[Test]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void deploy(IAppSPtr app) const override {
    cout << "[Deploy]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void report(IAppSPtr app) const override {
    cout << "[Report]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }

public:
  TestPlatformForIPhone7(vector<IOSSPtr> os_list)
    : ITestPlatform(TestDeviceFactory::createIPhone7(), os_list) {
  }
};

class TestPlatformForIPhone12 : public ITestPlatform {
protected:
  virtual void build(IAppSPtr app) const override {
    cout << "[Build]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void test(IAppSPtr app) const override {
    cout << "[Test]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void deploy(IAppSPtr app) const override {
    cout << "[Deploy]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }
  virtual void report(IAppSPtr app) const override {
    cout << "[Report]" << R"(")" << app->description() << R"(")"
         << " on " << device->description() << endl;
  }

public:
  TestPlatformForIPhone12(vector<IOSSPtr> os_list)
    : ITestPlatform(TestDeviceFactory::createIPhone12(), os_list) {
  }
};

struct TestPlatformFactory {
  static ITestPlatformSPtr createTestPlatformForGalaxyS20() {
    vector<IOSSPtr> os_list {
      OSFactory::createGoogleAndroid10(),
      OSFactory::createGoogleAndroid11()
    };
    return make_shared<TestPlatformForGalaxyS20>(os_list);
  }
  static ITestPlatformSPtr createTestPlatformForGalaxyS21() {
    vector<IOSSPtr> os_list {
      OSFactory::createGoogleAndroid10(),
      OSFactory::createGoogleAndroid11()
    };
    return make_shared<TestPlatformForGalaxyS20>(os_list);
  }
  static ITestPlatformSPtr createTestPlatformForIPhone7() {
    vector<IOSSPtr> os_list {
      OSFactory::createAppleIOS7(),
    };
    return make_shared<TestPlatformForGalaxyS20>(os_list);
  }
  static ITestPlatformSPtr createTestPlatformForIPhone12() {
    vector<IOSSPtr> os_list {
      OSFactory::createAppleIOS7(),
      OSFactory::createAppleIOS8()
    };
    return make_shared<TestPlatformForGalaxyS20>(os_list);
  }
};

// -------------------------------------------

void test() {
  vector<IAppSPtr> apps {
    AppFactory::createHealthApp(),
    AppFactory::createGameApp(),
    AppFactory::createMusicApp()
  };

  vector<ITestPlatformSPtr> targets {
    TestPlatformFactory::createTestPlatformForGalaxyS20(),
    TestPlatformFactory::createTestPlatformForGalaxyS21(),
    TestPlatformFactory::createTestPlatformForIPhone7(),
    TestPlatformFactory::createTestPlatformForIPhone12()
  };

  for_each(targets.begin(), targets.end(), [&apps](auto target) {
    for_each(apps.begin(), apps.end(), [&target](auto app) {
      target->run(app);
    });
  });
}

int main() {
  test();

  return 0;
}

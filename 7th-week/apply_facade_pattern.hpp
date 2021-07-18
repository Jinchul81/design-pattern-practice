#include "mock_app_test.hpp"

struct MockApps {
  std::vector<IAppSPtr> apps;
  MockApps() : apps {
    AppFactory::createHealthApp(),
    AppFactory::createGameApp(),
    AppFactory::createMusicApp()
  } {}

  const std::vector<IAppSPtr>& load() const {
    return apps;
  }
};

struct MockTestPlatforms {
  std::vector<ITestPlatformSPtr> targets;
  MockTestPlatforms() : targets {
    TestPlatformFactory::createTestPlatformForGalaxyS20(),
    TestPlatformFactory::createTestPlatformForGalaxyS21(),
    TestPlatformFactory::createTestPlatformForIPhone7(),
    TestPlatformFactory::createTestPlatformForIPhone12()
  } {}

  const std::vector<ITestPlatformSPtr>& load() const {
    return targets;
  }
};
  
class MockAppTestPlatform {
public:
  void run() const {
    MockApps mockApps;
    MockTestPlatforms mockTestPlatforms;
    const auto& apps = mockApps.load();
    const auto& targets = mockTestPlatforms.load();
    for_each(targets.begin(), targets.end(), [&apps](auto target) {
      for_each(apps.begin(), apps.end(), [&target](auto app) {
        target->run(app);
      });
    });
  }
};

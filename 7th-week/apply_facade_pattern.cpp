#include "apply_facade_pattern.hpp"

using namespace std;

void test() {
  MockAppTestPlatform mockAppTestPlatform;
  mockAppTestPlatform.run();
}

int main() {
  test();

  return 0;
}

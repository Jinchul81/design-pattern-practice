#include <iostream>
#include <type_traits>
#include <algorithm>
#include <exception>

using namespace std;

template <typename T1, typename T2>
void test(const vector<pair<T1, T2>>& input) {
  auto print = [](const auto& o) {
    for (const auto& e:o) cout << e << " ";
    cout << endl;
  };
  auto add_op = [](const auto& e) {
    return e.first + e.second;
  };
  auto subtract_op = [](const auto& e) {
    return e.first - e.second;
  };
  auto multiply_op = [](const auto& e) {
    return e.first * e.second;
  };
  auto divide_op = [](const auto& e) {
    if (!e.second) throw invalid_argument("Divisor cannot be zero.");
    return e.first / e.second;
  };

  {
    vector<common_type_t<T1, T2>> out(input.size());
    transform(input.begin(), input.end(), out.begin(), add_op);
    print(out);
  }
  {
    vector<common_type_t<T1, T2>> out(input.size());
    transform(input.begin(), input.end(), out.begin(), subtract_op);
    print(out);
  }
  {
    vector<common_type_t<T1, T2>> out(input.size());
    transform(input.begin(), input.end(), out.begin(), multiply_op);
    print(out);
  }
  {
    vector<common_type_t<T1, T2>> out(input.size());
    transform(input.begin(), input.end(), out.begin(), divide_op);
    print(out);
  }
}

int main() {
  test<long, int>({{1e11, 3}, {1e12, 4}, {1e13, 5}, {1e14, 6}});
  test<long, double>({{1, 2.3}, {2, 3.4}, {3, 4.5}, {4, 5.6}});

  return 0;
}

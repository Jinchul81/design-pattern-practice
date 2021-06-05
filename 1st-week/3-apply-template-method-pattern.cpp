#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct BinaryOp {
  virtual ~BinaryOp() = default;

  // template method
  virtual RET eval(const T1& lhs, const T2& rhs) final {
    check(lhs, rhs);
    return _eval(lhs, rhs);
  }

  // hook methods
  virtual void check(const T1& lhs, const T2& rhs) {
  };

  // abstract methods
  virtual RET _eval(const T1& lhs, const T2& rhs) = 0;
};

template <typename T1,
          typename T2>
using BinaryOpUPtr = unique_ptr<BinaryOp<T1, T2>>;

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct AddOp : public BinaryOp<T1, T2, RET> {
  virtual ~AddOp() = default;

  // abstract methods
  virtual RET _eval(const T1& lhs, const T2& rhs) override {
    return lhs + rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct SubtractOp : public BinaryOp<T1, T2, RET> {
  virtual ~SubtractOp() = default;

  virtual RET _eval(const T1& lhs, const T2& rhs) override {
    return lhs - rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct MultiplyOp : public BinaryOp<T1, T2, RET> {
  virtual ~MultiplyOp() = default;

  virtual RET _eval(const T1& lhs, const T2& rhs) override {
    return lhs * rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct DivideOp : public BinaryOp<T1, T2, RET> {
  virtual ~DivideOp() = default;

  virtual void check(const T1& lhs, const T2& rhs) override {
    if (!rhs) throw invalid_argument("Divisor cannot be zero.");
  }

  virtual RET _eval(const T1& lhs, const T2& rhs) override {
    return lhs / rhs;
  }
};

template <typename T1, typename T2>
void test(const vector<pair<T1, T2>>& input) {
  vector<BinaryOpUPtr<T1, T2>> ops;
  ops.push_back(make_unique<AddOp<T1, T2>>());
  ops.push_back(make_unique<SubtractOp<T1, T2>>());
  ops.push_back(make_unique<MultiplyOp<T1, T2>>());
  ops.push_back(make_unique<DivideOp<T1, T2>>());
  for_each(ops.begin(), ops.end(), [&input](const auto& op) {
    for_each(input.begin(), input.end(), [&op](const auto& e) {
      cout << op->eval(e.first, e.second) << " ";
    });
    cout << endl;
  });
}

int main() {
  test<long, int>({{1e11, 3}, {1e12, 4}, {1e13, 5}, {1e14, 6}});
  test<long, double>({{1, 2.3}, {2, 3.4}, {3, 4.5}, {4, 5.6}});

  return 0;
}

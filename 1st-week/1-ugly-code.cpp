#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>
#include <cassert>

using namespace std;

template <typename T1,
          typename T2,
          typename RET>
class BinaryOp {
  RET add(const T1& lhs, const T2& rhs) const {
    return lhs + rhs;
  }

  RET subtract(const T1& lhs, const T2& rhs) const {
    return lhs - rhs;
  }

  RET multiply(const T1& lhs, const T2& rhs) const {
    return lhs * rhs;
  }

  RET divide(const T1& lhs, const T2& rhs) const {
    if (!rhs) throw invalid_argument("Divisor cannot be zero.");
    return lhs / rhs;
  }

protected:
  enum class Op { Add, Subtract, Multiply, Divide };
  Op op;

public:
  explicit BinaryOp(const Op& op)
    : op(op) {
  }

  virtual ~BinaryOp() = default;

  RET eval(const T1& lhs, const T2& rhs) const {
    switch (op) {
    case Op::Add:
      return add(lhs, rhs);
    case Op::Subtract:
      return subtract(lhs, rhs);
    case Op::Multiply:
      return multiply(lhs, rhs);
    case Op::Divide:
      return divide(lhs, rhs);
    }
    assert(0);
    return 0;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class AddOp : public BinaryOp<T1, T2, RET> {
public:
  AddOp()
    : BinaryOp<T1, T2, RET>(BinaryOp<T1, T2, RET>::Op::Add) {
    }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class SubtractOp : public BinaryOp<T1, T2, RET> {
public:
  SubtractOp()
    : BinaryOp<T1, T2, RET>(BinaryOp<T1, T2, RET>::Op::Subtract) {
    }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class MultiplyOp : public BinaryOp<T1, T2, RET> {
public:
  MultiplyOp()
    : BinaryOp<T1, T2, RET>(BinaryOp<T1, T2, RET>::Op::Multiply) {
    }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class DivideOp : public BinaryOp<T1, T2, RET> {
public:
  DivideOp()
    : BinaryOp<T1, T2, RET>(BinaryOp<T1, T2, RET>::Op::Divide) {
    }
};

template <typename OP,
          typename T>
void print(const OP& op, const T& input) {
  for (auto& e:input) {
    cout << op.eval(e.first, e.second) << " ";
  }
  cout << endl;
}

template <typename T1, typename T2>
void test(const vector<pair<T1, T2>>& input) {
  AddOp<T1, T2> add_op;
  SubtractOp<T1, T2> subtract_op;
  MultiplyOp<T1, T2> multiply_op;
  DivideOp<T1, T2> divide_op;

  print(add_op, input);
  print(subtract_op, input);
  print(multiply_op, input);
  print(divide_op, input);
}

int main() {
  test<long, int>({{1e11, 3}, {1e12, 4}, {1e13, 5}, {1e14, 6}});
  test<long, double>({{1, 2.3}, {2, 3.4}, {3, 4.5}, {4, 5.6}});

  return 0;
}

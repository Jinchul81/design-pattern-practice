#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>

using namespace std;

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
struct BinaryOp {
  virtual ~BinaryOp() = default;

  virtual RET eval(const T1& lhs, const T2& rhs) = 0;
};

template <typename T1,
          typename T2>
using BinaryOpSPtr = shared_ptr<BinaryOp<T1, T2>>;

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class AddOp : public BinaryOp<T1, T2, RET> {
public:
  virtual ~AddOp() = default;

  virtual RET eval(const T1& lhs, const T2& rhs) override {
    return lhs + rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class SubtractOp : public BinaryOp<T1, T2, RET> {
public:
  virtual ~SubtractOp() = default;

  virtual RET eval(const T1& lhs, const T2& rhs) override {
    return lhs - rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class MultiplyOp : public BinaryOp<T1, T2, RET> {
public:
  virtual ~MultiplyOp() = default;

  virtual RET eval(const T1& lhs, const T2& rhs) override {
    return lhs * rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class DivideOp : public BinaryOp<T1, T2, RET> {
public:
  virtual ~DivideOp() = default;

  virtual RET eval(const T1& lhs, const T2& rhs) override {
    if (!rhs) throw invalid_argument("Divisor cannot be zero.");
    return lhs / rhs;
  }
};

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class Context {
  BinaryOpSPtr<T1, T2> op_ptr;

public:
  virtual ~Context() = default;

  void set_operator(const BinaryOpSPtr<T1, T2>& new_op_ptr) {
    op_ptr = new_op_ptr;
  }

  const BinaryOpSPtr<T1, T2>& get_operator() const {
    if (!op_ptr) throw runtime_error("Op has not been set.");
    return op_ptr;
  }
};

template <typename T1,
          typename T2>
using ContextSPtr = shared_ptr<Context<T1, T2>>;

template <typename T1,
          typename T2,
          typename RET = common_type_t<T1, T2>>
class Calculator {
  ContextSPtr<T1, T2> context_ptr;

public:
  Calculator(const ContextSPtr<T1, T2>& new_context)
    : context_ptr(new_context) {
  }

  virtual ~Calculator() = default;

  RET eval(const T1& lhs, const T2& rhs) const {
    if (!context_ptr) throw runtime_error("Context has not been set.");
    return context_ptr->get_operator()->eval(lhs, rhs);
  }
};

template <typename CALC,
          typename T>
void print(const CALC& calc, const T& input) {
  for (auto& e:input) {
    cout << calc.eval(e.first, e.second) << " ";
  }
  cout << endl;
}

template <typename T1, typename T2>
void test(const vector<pair<T1, T2>>& input) {
  ContextSPtr<T1, T2> context_ptr = make_shared<Context<T1, T2>>();
  Calculator<T1, T2> calc(context_ptr);
  auto add_op = make_shared<AddOp<T1, T2>>();
  auto subtract_op = make_shared<SubtractOp<T1, T2>>();
  auto multiply_op = make_shared<MultiplyOp<T1, T2>>();
  auto divide_op = make_shared<DivideOp<T1, T2>>();

  context_ptr->set_operator(add_op);
  print(calc, input);
  context_ptr->set_operator(subtract_op);
  print(calc, input);
  context_ptr->set_operator(multiply_op);
  print(calc, input);
  context_ptr->set_operator(divide_op);
  print(calc, input);
}

int main() {
  test<long, int>({{1e11, 3}, {1e12, 4}, {1e13, 5}, {1e14, 6}});
  test<long, double>({{1, 2.3}, {2, 3.4}, {3, 4.5}, {4, 5.6}});

  return 0;
}

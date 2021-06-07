#include <iostream>
#include <random>

template <typename T>
struct IGenerator {
  virtual ~IGenerator() = default;
  virtual T get() = 0;
};

template <typename T>
class RandomGenerator : public IGenerator<T> {
protected:
  static constexpr auto DEFAULT_SEED= 1729u;
  std::mt19937 gen;

  RandomGenerator(uint_fast32_t seed = DEFAULT_SEED)
    : gen(seed) {
  }
};

template <typename T = double>
class RandomFloatingGenerator : public RandomGenerator<T> {
  std::uniform_real_distribution<T> dist;

protected:
  T next() {
    return dist(RandomGenerator<T>::gen);
  }

public:
  RandomFloatingGenerator(T from, T to, uint_fast32_t seed)
    : RandomGenerator<T>(seed), dist(from, to) {
  }

  RandomFloatingGenerator(T from, T to)
    : RandomGenerator<T>(), dist(from, to) {
  }

  virtual ~RandomFloatingGenerator() = default;
  virtual T get() override {
    return next();
  }
};

template <typename T = int>
class RandomNumericGenerator : public RandomGenerator<T> {
  std::uniform_int_distribution<T> dist;

protected:
  T next() {
    return dist(RandomGenerator<T>::gen);
  }

public:
  RandomNumericGenerator(T from, T to, uint_fast32_t seed)
    : RandomGenerator<T>(seed), dist(from, to) {
  }

  RandomNumericGenerator(T from, T to)
    : RandomGenerator<T>(), dist(from, to) {
  }

  virtual ~RandomNumericGenerator() = default;
  virtual T get() override {
    return next();
  }
};

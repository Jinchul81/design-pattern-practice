#include <iostream>

using namespace std;

enum class Direction {
  kLeft = 0, kCenter = 1, kRight = 2
};

class Bicycle {
  Direction direction = Direction::kCenter;

public:
  void push_pedals() const {
    cout << "Move ";
    if (direction == Direction::kLeft)
      cout << "left";
    else if (direction == Direction::kCenter)
      cout << "forward";
    else if (direction == Direction::kRight)
      cout << "right";
    else
      throw runtime_error("Invalid direction type.");
    cout << " direction" << endl;
  }

  void set_handle(const Direction d) {
    direction = d;
  }
};

struct IBoat {
  virtual void run_engine() const = 0; 
  virtual void move_steer(const Direction d) = 0;
};

class BicycleBoat : public IBoat {
  Bicycle bicycle; 

public:
  virtual void run_engine() const override {
    cout << "Run boat engine: ";
    bicycle.push_pedals();
  }

  virtual void move_steer(const Direction d) override {
    bicycle.set_handle(d);
  }
};

void test() {
  BicycleBoat funnyBoat;
  funnyBoat.run_engine();
  funnyBoat.move_steer(Direction::kLeft);
  funnyBoat.run_engine();
  funnyBoat.move_steer(Direction::kRight);
  funnyBoat.run_engine();
  funnyBoat.move_steer(Direction::kCenter);
  funnyBoat.run_engine();
}

int main() {
  test();
  return 0;
}

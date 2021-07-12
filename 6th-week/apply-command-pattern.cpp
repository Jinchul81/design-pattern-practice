#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

using namespace std;

enum class TaskPriority {
  Light           = 100,
  AirConditioner  = 80,
  Food            = 70,
  DishWashing     = 60,
  VaccumCleaner   = 50,
  WashingMachine  = 45,
  Rest            = 40,
  CarWashing      = 30,
  Postbox         = 20,
  Garden          = 10
};

struct ITask {
  const TaskPriority score;
  virtual void execute() const = 0;
  virtual void print_log() const = 0;

  ITask(TaskPriority score)
    : score(score) {
  }

  virtual ~ITask() = default;
  TaskPriority get_score() const { return score; }
};
using ITaskSPtr = shared_ptr<ITask>;

class TaskLight : public ITask {
  mutable bool on = false;

  virtual void print_log() const override {
    cout << "The light has been turned"
         << (on ? " on" : " off")
         << "." << endl;
  }

public:
  virtual void execute() const override {
    on ^= 1;
    print_log();
  }

  TaskLight()
    : ITask(TaskPriority::Light) {
  }
};

class TaskAirConditioner : public ITask {
  mutable bool on = false;

  virtual void print_log() const override {
    cout << "The air conditioner has been turned"
         << (on ? " on" : " off")
         << "." << endl;
  }

public:
  virtual void execute() const override {
    on ^= 1;
    print_log();
  }

  TaskAirConditioner()
    : ITask(TaskPriority::AirConditioner) {
  }
};

class TaskFoodPreparation : public ITask {
  virtual void print_log() const override {
    cout << "The food has been prepared." << endl;
  }

public:
  virtual void execute() const override {
    print_log();
  }

  TaskFoodPreparation()
    : ITask(TaskPriority::Food) {
  }
};

class TaskDishWashing : public ITask {
  mutable bool on = false;

  virtual void print_log() const override {
    cout << "The dish washing has been turned"
         << (on ? " on" : " off")
         << "." << endl;
  }

public:
  virtual void execute() const override {
    on ^= 1;
    print_log();
  }

  TaskDishWashing()
    : ITask(TaskPriority::WashingMachine) {
  }
};

class TaskVaccumCleaner : public ITask {
  mutable bool on = false;

  virtual void print_log() const override {
    cout << "The vaccum cleaner has been turned"
         << (on ? " on" : " off")
         << "." << endl;
  }

public:
  virtual void execute() const override {
    on ^= 1;
    print_log();
  }

  TaskVaccumCleaner()
    : ITask(TaskPriority::VaccumCleaner) {
  }
};

class TaskWashingMachine : public ITask {
  mutable bool on = false;

  virtual void print_log() const override {
    cout << "The wahsing machine has been turned"
         << (on ? " on" : " off")
         << "." << endl;
  }

public:
  virtual void execute() const override {
    on ^= 1;
    print_log();
  }

  TaskWashingMachine()
    : ITask(TaskPriority::WashingMachine) {
  }
};

class TaskTakingRest : public ITask {
  virtual void print_log() const override {
    cout << "Just taking a rest." << endl;
  }

public:
  virtual void execute() const override {
    print_log();
  }

  TaskTakingRest()
    : ITask(TaskPriority::Rest) {
  }
};

class TaskCarWashing : public ITask {
  virtual void print_log() const override {
    cout << "Let's go to wash my car!" << endl;
  }

public:
  virtual void execute() const override {
    print_log();
  }

  TaskCarWashing()
    : ITask(TaskPriority::CarWashing) {
  }
};

class TaskCheckPostbox: public ITask {
  virtual void print_log() const override {
    cout << "Let's check my postbox." << endl;
  }

public:
  virtual void execute() const override {
    print_log();
  }

  TaskCheckPostbox()
    : ITask(TaskPriority::Postbox) {
  }
};

class TaskWaterGarden: public ITask {
  virtual void print_log() const override {
    cout << "Let's water my garden." << endl;
  }

public:
  virtual void execute() const override {
    print_log();
  }

  TaskWaterGarden()
    : ITask(TaskPriority::Garden) {
  }
};

class TaskManager {
  vector<ITaskSPtr> tasks;
  long long pos = 0;
  const long long num_tasks;

  void sort() {
    std::sort(tasks.begin(), tasks.end(), [](auto&l, auto&r) {
      return l->get_score() > r->get_score();
    });
  }

public:
  TaskManager(const vector<ITaskSPtr>& tasks)
    : tasks(tasks.begin(), tasks.end()), num_tasks(tasks.size()) {
    sort();
  }

  bool do_next_task() {
    if (pos+1 == num_tasks) return false;
    tasks[pos]->execute();
    if (pos+1 < num_tasks) ++pos;
    return true;
  }

  bool do_prev_task() {
    if (pos-1 < 0) return false;
    tasks[pos]->execute();
    if (pos-1 > -1) --pos;
    return true;
  }

  bool move_forward(long long v) {
    if (pos+v >= num_tasks) return false;
    pos += v;
    return true;
  }

  bool move_backward(long long v) {
    if (pos-v < 0) return false;
    pos -= v;
    return true;
  }
};

void test() {
  vector<ITaskSPtr> tasks {
    make_shared<TaskWaterGarden>(),
    make_shared<TaskCheckPostbox>(),
    make_shared<TaskCarWashing>(),
    make_shared<TaskTakingRest>(),
    make_shared<TaskWashingMachine>(),
    make_shared<TaskVaccumCleaner>(),
    make_shared<TaskDishWashing>(),
    make_shared<TaskFoodPreparation>(),
    make_shared<TaskAirConditioner>(),
    make_shared<TaskLight>()
  };
  shuffle(tasks.begin(), tasks.end(), mt19937(random_device()()));
  TaskManager task_manager(tasks);
  cout << "* Do all the tasks by higher priority!" << endl;
  while (task_manager.do_next_task());
  cout << "* Do all the tasks by smaller priority!" << endl;
  while (task_manager.do_prev_task());
  cout << "* Repeat the last twenty tasks again!" << endl;
  if (task_manager.move_forward(20))
    while (task_manager.do_next_task());
  else
    cout << "* Failed" << endl;
  cout << "* Repeat the first six tasks again!" << endl;
  if (task_manager.move_forward(0))
    for (auto i = 0; i < 6 and task_manager.do_next_task(); ++i);
  else
    cout << "* Failed" << endl;
  cout << "* Repeat the last six tasks again!" << endl;
  if (task_manager.move_backward(2))
    for (auto i = 0; i < 6 and task_manager.do_next_task(); ++i);
  else
    cout << "* Failed" << endl;
}

int main() {
  test();

  return 0;
}

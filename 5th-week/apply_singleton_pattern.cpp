#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>
#include <sstream>

#include "random.hpp"

using namespace std;

class ICharacterStat {
  int hp = 0;
  int money = 0;
  int power = 0;

protected:
  ICharacterStat(int hp, int money, int power)
    : hp(hp), money(money), power(power) {
  }

  virtual ostream& dump(ostream& o) const {
    o << "HP: " << hp << ", Money: " << money << ", Power: " << power;
    return o;
  }

  void set_hp(int new_hp) { hp = new_hp; }

public:
  int get_hp() const { return hp; }
  int get_money() const { return money; }
  int get_power() const { return power; }

  void add_hp(int v) { hp += v; }
  void add_money(int v) { money += v; }
  void add_power(int v) { power += v; }

  bool is_alive() {
    return get_hp() > 0;
  }

  void print() {
    ostringstream oss;
    dump(oss);
    cout << oss.str() << endl;
  }
};

class Warrior : public ICharacterStat {
private:
  static constexpr auto DEFAULT_HP = 500;
  static constexpr auto DEFAULT_MONEY = 1e3;
  static constexpr auto DEFAULT_POWER = 15;

  Warrior()
    : ICharacterStat(DEFAULT_HP, DEFAULT_MONEY, DEFAULT_POWER) {
  }

  virtual ostream& dump(ostream& o) const override {
    cout << "[Warrior] ";
    ICharacterStat::dump(o);
    return o;
  }

public:
  static Warrior& get() {
    static unique_ptr<Warrior> warrior;
    if (! warrior) {
      mutex mtx;
      scoped_lock<mutex>{mtx};
      struct MakeSharedEnabler : public Warrior {};
      warrior = make_unique<MakeSharedEnabler>();
    }
    return *warrior.get();
  }

  void reset_hp() {
    set_hp(DEFAULT_HP);
  }
};

class Monster : public ICharacterStat {
  Monster(int hp, int money, int power)
    : ICharacterStat(hp, money, power) {
  }

  virtual ostream& dump(ostream& o) const override {
    cout << "[Monster] ";
    ICharacterStat::dump(o);
    return o;
  }

public:
  static Monster create() {
    static constexpr auto from = 3, to = 15;
    static RandomNumericGenerator rng(from, to);
    auto hp = rng.get(), money = rng.get(), power = rng.get();
    return Monster(hp, money, power);
  }
};

void print(string_view msg, ICharacterStat& target) {
  cout << "* " << msg << endl;
  target.print();
}

// Can increase power if buy an item using money.
void visit_store() {
  static constexpr auto cost = 2000, stat = 5;
  Warrior& hero = Warrior::get();
  if (hero.get_money() > cost) {
    hero.add_money(-cost);
    hero.add_power(stat);
    print("Hero bought an item and increased his power!", hero);
  }
}

// Reset HP
void visit_inn() {
  static constexpr auto cost = 100;
  Warrior& hero = Warrior::get();
  if (hero.get_money() > cost) {
    hero.add_money(-cost);
    hero.reset_hp();
    print("Hero took a sleep and his HP has been recovered!", hero);
  }
}

// Hunt monster and earn money
void hunt_monster(Monster& monster) {
  Warrior& hero = Warrior::get();
  while (monster.is_alive() and hero.is_alive()) {
    monster.add_hp(-hero.get_power());
    hero.add_hp(-monster.get_power());
  }
  if (hero.is_alive()) {
    hero.add_money(monster.get_money());
    print("Hero won the fight! He earned some money.", hero);
  }
}

void test() {
  static constexpr auto hp_limit = 100;
  Warrior& hero = Warrior::get();
  print("Hero has been created...!", hero);
  while (true) {
    Monster monster = Monster::create();
    print("Found a monster...", monster);
    hunt_monster(monster);
    if (! hero.is_alive()) break;
    if (hero.get_hp() < hp_limit)
      visit_inn();
    visit_store();
  }
  print("Game end...", hero);
}

int main() {
  test();

  return 0;
}

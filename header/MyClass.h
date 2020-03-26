#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
using namespace std;

#define INF -1
#define WARRIOR_NUM 5
#define WEAPON_NUM 3
#define HEADQUARTER_NUM 2
#define WEAPON_LIMIT 10
#define WARRIOR_LIMIT 1000

enum WeaponType { SWORD, BOMB, ARROW, UNDEFINED };
enum WarriorType { DRAGON, NINJA, ICEMAN, LION, WOLF };
enum HeadquarterType { RED, BLUE };

const string Time();
const string String(const WeaponType &type);
const string String(const WarriorType &type);
const string String(const HeadquarterType &type);

int global_time = 0;

class Fraction;
class Weapon;
class WeaponPackage;
class Warrior;
class City;
class Headquarter;
class Map;
const string weapon_name[WEAPON_NUM] = {"sword", "bomb", "arrow"};
const string warrior_name[WARRIOR_NUM] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const string headquarter_name[HEADQUARTER_NUM] = {"red", "blue"};
class Fraction {
public:
    int numerator, denominator;
    Fraction(const int &_numerator, const int &_denominator = 100);
    friend int operator*(const int &n, const Fraction &f);
};

class Weapon {
public:
    WeaponType type;
    Fraction damage_multiplier;
    int durability;
    Weapon(const WeaponType &_type = UNDEFINED, const int &_percentage = 0, const int &_durability = INF);
    void Wear();
    friend bool operator<(const Weapon &w1, const Weapon &w2);
};
const Weapon weapon_template[WEAPON_NUM] = {{SWORD, 20}, {BOMB, 40, 1}, {ARROW, 30, 2}};

class WeaponPackage {
public:
    Weapon weapon[WEAPON_LIMIT];
    int tot, cur;
    WeaponPackage();
    const Weapon &operator[](const int &id);
    bool Consumable();
    void Reset();
    void Scroll();
    bool Empty();
    bool Full();
    void Wear();
    void Add(const int &n);
    void Add(const Weapon &w);
    void Remove(const int &id);
    void Sort();
    WeaponType Type(const int &id);
    Fraction DamageMultiplier();
    const string Info();
};

class Warrior {
public:
    int num, str, atk;
    bool alive;
    WarriorType type;
    Headquarter *owner;
    City *pos;
    WeaponPackage weapon;
    Warrior(Headquarter *_owner);
    virtual ~Warrior();
    bool Alive();
    const string Name();
    const string Info();
    const string Pos();
    virtual int Attack(Warrior &w);
    virtual void Hurted(const int &n);
    virtual void Move();
    pair<int, WeaponType> TakeWeapon(Warrior &w);
    virtual void Take(Warrior &w);
};

class Dragon : public Warrior {
public:
    Dragon(Headquarter *_owner);
    int Attack(Warrior &w);
    void Hurted(const int &n);
    void Take(Warrior &w);
    void Move();
    void Yell();
};

class Ninja : public Warrior {
public:
    Ninja(Headquarter *_owner);
    int Attack(Warrior &w);
    void Hurted(const int &n);
    void Take(Warrior &w);
    void Move();
};

class Iceman : public Warrior {
public:
    Iceman(Headquarter *_owner);
    int Attack(Warrior &w);
    void Hurted(const int &n);
    void Take(Warrior &w);
    void Move();
};

class Lion : public Warrior {
public:
    int loyalty;
    static int loyalty_decay;
    Lion(Headquarter *_owner);
    int Attack(Warrior &w);
    void Hurted(const int &n);
    void Take(Warrior &w);
    void Move();
    void TryRun();
    const string Loyalty();
};

int Lion::loyalty_decay;

class Wolf : public Warrior {
public:
    Wolf(Headquarter *_owner);
    int Attack(Warrior &w);
    void Hurted(const int &n);
    void Take(Warrior &w);
    void Move();
    void Snatch(Warrior &w);
};

class City {
public:
    Warrior *warrior[HEADQUARTER_NUM];
    Map *pos;
    int num;
    City(Map *_pos, const int &n);
    virtual ~City();
    City *Next();
    City *Prev();
    void Battle();
};

class Headquarter : public City {
public:
    Warrior *warrior_seq[WARRIOR_LIMIT];
    WarriorType produce_seq[WARRIOR_NUM];
    int warrior_str[WARRIOR_NUM];
    int warrior_atk[WARRIOR_NUM];
    HeadquarterType type;
    int warrior_cnt;
    int cur, str;
    bool stop_create;
    Headquarter(Map *_pos, const HeadquarterType &_type, const int &n, WarriorType *_seq);
    ~Headquarter();
    const string Name();
    void Create();
    bool CheckConquered();
    void CheckState();
};

class Map {
public:
    City **city;
    Headquarter *red, *blue;
    int city_cnt;
    int headquarter_str;
    int str[WARRIOR_NUM];
    int atk[WARRIOR_NUM];
    Map(const int &n, const int &_strength, int _health[WARRIOR_NUM], int _attack[WARRIOR_NUM]);
    ~Map();
    void CreateHeadquarter();
    void Move();
    void CheckState();
    void CheckWarrior();
    void CheckLion();
    void CheckSnatch();
    void Battle();
};
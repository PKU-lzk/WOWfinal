#include <set>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

#define INF -1
#define WARRIOR_NUM 5
#define WEAPON_NUM 3

enum WeaponType { SWORD, BOMB, ARROW };
enum WarriorType { DRAGON, NINJA, ICEMAN, LION, WOLF };
enum HeadquarterType { RED, BLUE, UNDEFINED };
vector<string> weapon_name = {"sword", "bomb", "arrow"};
vector<string> warrior_name = {"dragon", "ninja", "iceman", "lion", "wolf"};
vector<string> headquarter_name = {"red", "blue"};

string s(WeaponType t);
string s(WarriorType t);
string s(HeadquarterType t);

int global = 0;
string Time();

class Weapon;
class Package;
class Warrior;
class City;
class Headquarter;
class Map;

class Weapon {
public:
    WeaponType type;
    int dmg;
    int dura;
    Weapon(WeaponType t, int _dmg, int _dura);
    virtual void Wear();
    virtual int Use();
    virtual bool Broken();
    string Info();
};
class Sword : public Weapon {
public:
    Sword(int _dmg);
    void Wear();
};

class Bomb : public Weapon {
public:
    static int dmg;
    Bomb(int _dmg = dmg);
};

int Bomb::dmg = INF;

class Arrow : public Weapon {
public:
    static int dmg;
    Arrow(int _dmg = dmg);
};

int Arrow::dmg;

class Warrior {
public:
    WarriorType type;
    int num, str, atk, tmp_str;
    bool move;
    Headquarter *owner;
    City *city;
    Weapon *pack[WEAPON_NUM];
    int WeaponDmg();
    int UseWeapon(WeaponType w);
    virtual void Arm(WeaponType t);
    Warrior(WarriorType t, Headquarter *_owner);
    string Name();
    string Info();
    bool Alive();
    virtual void Dead(Warrior *w, WeaponType t);
    virtual void Attack(Warrior *w, WeaponType t = SWORD);
    virtual void Counter(Warrior *w);
    virtual void Hurt(int n);
    virtual void Earn();
    virtual void Gain(int n = 8);
    virtual void Move();
    virtual void Report();
    virtual void Res(Warrior *w);
};

class Dragon : public Warrior {
public:
    double morale;
    Dragon(Headquarter *_owner);
    void Yell();
    void Res(Warrior *w);
};

class Ninja : public Warrior {
public:
    Ninja(Headquarter *_owner);
    void Counter(Warrior *w);
};

class Iceman : public Warrior {
public:
    int step;
    Iceman(Headquarter *_owner);
    void Move();
};

class Lion : public Warrior {
public:
    int loyalty;
    static int loyalty_decay;
    Lion(Headquarter *_owner);
    void Res(Warrior *w);
    void Dead(Warrior *w, WeaponType t = SWORD);
    void Run();
};

int Lion::loyalty_decay;

class Wolf : public Warrior {
public:
    Wolf(Headquarter *_owner);
    void TakeWeapon(Warrior *w);
};

class City {
public:
    HeadquarterType type;
    set<Warrior *> warrior[2];
    Map *map;
    int num;
    int elem;
    virtual string Name();
    HeadquarterType flag;
    HeadquarterType curr_winner;
    Warrior *Rank(int n);
    void Flag(HeadquarterType t = UNDEFINED);
    bool isCity();
    City *pos(int n);
    City(Map *_map, int n, HeadquarterType = UNDEFINED, int _elem = 0);
};

class Headquarter : public City {
public:
    static vector<int> str;
    static vector<int> atk;
    static WarriorType prod_seq[2][WARRIOR_NUM];
    static int beg_elem;
    vector<Warrior *> warr_seq;
    Headquarter(Map *_map, HeadquarterType t, int n);
    string Name();
    void Create();
    bool Conquered();
    void Report();
};

vector<int> Headquarter::str;
vector<int> Headquarter::atk;
WarriorType Headquarter::prod_seq[2][WARRIOR_NUM] = {{ICEMAN, LION, WOLF, NINJA, DRAGON},
                                                     {LION, DRAGON, NINJA, ICEMAN, WOLF}};
int Headquarter::beg_elem;

class Map {
public:
    vector<City *> city;
    vector<Headquarter *> headquarter;
    int num;
    bool end;
    Map(int _elem, int _num, vector<int> _str, vector<int> _atk);
    void Create();
    void LionRun();
    void Move();
    void Update();
    void Earn();
    void UseArrow();
    void UseBomb();
    void Battle();
    void Clear();
    void CheckElem();
    void CheckWeapon();
    void Begin(int t);
};
#include <algorithm>
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

int rd() {
    int t;
    scanf("%d", &t);
    return t;
}

int main() {
    int test_num = rd();
    for (int i = 1; i <= test_num; ++i) {
        printf("Case %d:\n", i);
        int M, N, R, K, T;
        scanf("%d%d%d%d%d", &M, &N, &R, &K, &T);
        Arrow::dmg = R;
        Lion::loyalty_decay = K;
        vector<int> str, atk;
        for (int j = 0; j < WARRIOR_NUM; ++j) str.push_back(rd());
        for (int j = 0; j < WARRIOR_NUM; ++j) atk.push_back(rd());
        Map m(M, N, str, atk);
        m.Begin(T);
    }
    return 0;
}

string Time() {
    char s[7];
    sprintf(s, "%03d:%02d", global / 60, global % 60);
    return s;
}

string s(WeaponType t) { return weapon_name[t]; }
string s(WarriorType t) { return warrior_name[t]; }
string s(HeadquarterType t) { return headquarter_name[t]; }

Weapon::Weapon(WeaponType t, int _dmg, int _dura) : type(t), dmg(_dmg), dura(_dura) {}
void Weapon::Wear() { --dura; }
int Weapon::Use() {
    int d = dmg;
    Wear();
    return d;
}
string Weapon::Info() {
    string t = s(type);
    if (type == SWORD) t += "(" + to_string(dmg) + ")";
    if (type == ARROW) t += "(" + to_string(dura) + ")";
    return t;
}
bool Weapon::Broken() { return dura == 0 || dmg == 0; }
Sword::Sword(int dmg) : Weapon(SWORD, dmg, -1) {}
void Sword::Wear() { dmg = dmg * 4 / 5; }
Bomb::Bomb(int dmg) : Weapon(BOMB, dmg, 1) {}
Arrow::Arrow(int dmg) : Weapon(ARROW, dmg, 3) {}

int Warrior::WeaponDmg() { return pack[SWORD] ? pack[SWORD]->dmg : 0; }
int Warrior::UseWeapon(WeaponType w) {
    if (!pack[w]) return 0;
    int d = pack[w]->Use();
    if (pack[w]->Broken()) delete pack[w], pack[w] = nullptr;
    return d;
}
Warrior::Warrior(WarriorType t, Headquarter *_owner) : type(t), move(false), owner(_owner) {
    num = owner->warr_seq.size() + 1;
    str = 0;
    Gain(owner->str[t]);
    atk = owner->atk[t];
    city = owner;
    memset(pack, 0, sizeof(pack));
    printf("%s %s born\n", Time().c_str(), Name().c_str());
}
bool Warrior::Alive() { return str; }
void Warrior::Dead(Warrior *w, WeaponType t) {
    printf("%s ", Time().c_str());
    if (t == SWORD) printf("%s was killed in %s\n", Name().c_str(), city->Name().c_str());
    if (t == BOMB) printf("%s used a bomb and killed %s\n", w->Name().c_str(), Name().c_str());
    if (t == ARROW) printf("%s shot and killed %s\n", w->Name().c_str(), Name().c_str());
}
string Warrior::Name() { return s(owner->type) + " " + s(type) + " " + to_string(num); }
string Warrior::Info() { return "with " + to_string(str) + " elements and force " + to_string(atk); }
void Warrior::Arm(WeaponType t) {
    if (t == SWORD && atk >= 5) pack[t] = new Sword(atk / 5);
    if (t == BOMB) pack[t] = new Bomb();
    if (t == ARROW) pack[t] = new Arrow();
}
void Warrior::Attack(Warrior *w, WeaponType t) {
    if (t == SWORD)
        w->Hurt(atk + UseWeapon(t)), printf("%s %s attacked %s in %s %s\n", Time().c_str(), Name().c_str(),
                                            w->Name().c_str(), city->Name().c_str(), Info().c_str());
    if (t == BOMB) UseWeapon(t), Hurt(str), w->Hurt(w->str);
    if (t == ARROW) w->Hurt(UseWeapon(t));
    if (!w->Alive())
        w->Dead(this, t);
    else if (t == ARROW)
        printf("%s %s shot\n", Time().c_str(), Name().c_str());
}
void Warrior::Counter(Warrior *w) {
    printf("%s %s fought back against %s in %s\n", Time().c_str(), Name().c_str(), w->Name().c_str(),
           city->Name().c_str());
    w->Hurt(atk / 2 + UseWeapon(SWORD));
    if (!w->Alive()) w->Dead(this, SWORD);
}
void Warrior::Hurt(int n) { str = (str > n) ? str - n : 0; }
void Warrior::Move() {
    City *next = city->pos((owner->type == RED) ? 1 : -1);
    if (move || !next) return;
    city->warrior[owner->type].erase(this);
    next->warrior[owner->type].insert(this);
    city = next;
    move = true;
}
void Warrior::Res(Warrior *w) {}
void Warrior::Gain(int n) {
    if (owner->elem >= n) owner->elem -= n, str += n;
}
void Warrior::Earn() {
    if (!city->elem) return;
    printf("%s %s earned %d elements for his headquarter\n", Time().c_str(), Name().c_str(), city->elem);
    owner->elem += city->elem, city->elem = 0;
}
void Warrior::Report() {
    string info = "";
    for (auto w : pack)
        if (w) info = w->Info() + "," + info;
    if (info == "")
        info = "no weapon";
    else
        info.pop_back();
    printf("%s %s has %s\n", Time().c_str(), Name().c_str(), info.c_str());
}

Dragon::Dragon(Headquarter *_owner) : Warrior(DRAGON, _owner), morale(1.0 * owner->elem / str) {
    Arm(WeaponType(num % 3));
    printf("Its morale is %.2f\n", morale);
}
void Dragon::Yell() {
    if (Alive() && morale > 0.8) printf("%s %s yelled in %s\n", Time().c_str(), Name().c_str(), city->Name().c_str());
}
void Dragon::Res(Warrior *w) { w->Alive() ? (morale -= 0.2) : (morale += 0.2); }

Ninja::Ninja(Headquarter *_owner) : Warrior(NINJA, _owner) {
    Arm(WeaponType(num % 3));
    Arm(WeaponType((num + 1) % 3));
}
void Ninja::Counter(Warrior *w) {}

Iceman::Iceman(Headquarter *_owner) : Warrior(ICEMAN, _owner), step(0) { Arm(WeaponType(num % 3)); }
void Iceman::Move() {
    if (move) return;
    Warrior::Move();
    step ^= 1;
    if (step == 0) {
        atk += 20;
        str -= 9;
        if (str <= 0) str = 1;
    }
}

Lion::Lion(Headquarter *_owner) : Warrior(LION, _owner), loyalty(owner->elem) {
    printf("Its loyalty is %d\n", loyalty);
}
void Lion::Res(Warrior *w) {
    if (w->Alive()) loyalty -= loyalty_decay;
}
void Lion::Dead(Warrior *w, WeaponType t) {
    Warrior::Dead(w, t);
    if (t == SWORD) w->Hurt(-tmp_str);
}
void Lion::Run() {
    if (loyalty <= 0)
        printf("%s %s ran away\n", Time().c_str(), Name().c_str()), city->warrior[owner->type].erase(this);
}

Wolf::Wolf(Headquarter *_owner) : Warrior(WOLF, _owner) {}
void Wolf::TakeWeapon(Warrior *w) {
    for (int i = 0; i < WEAPON_NUM; ++i)
        if (!pack[i]) swap(pack[i], w->pack[i]);
}

Warrior *City::Rank(int n) {
    bool rank = (n & 1) ^ (flag == RED || (flag == UNDEFINED && num % 2 == 1)) ^ 1;
    return warrior[rank].empty() ? nullptr : *warrior[rank].begin();
}
City::City(Map *_map, int n, HeadquarterType t, int _elem)
    : type(t), map(_map), num(n), elem(_elem), flag(UNDEFINED), curr_winner(flag) {}
string City::Name() { return "city " + to_string(num); }
bool City::isCity() { return type == UNDEFINED; }
City *City::pos(int n) { return (num + n >= 0 && num + n <= map->num + 1) ? map->city[num + n] : nullptr; }
void City::Flag(HeadquarterType t) {
    if (t != UNDEFINED && t == curr_winner && t != flag)
        flag = t, printf("%s %s flag raised in %s\n", Time().c_str(), s(t).c_str(), Name().c_str());
    curr_winner = t;
}
Headquarter::Headquarter(Map *_map, HeadquarterType t, int n) : City(_map, n, t, beg_elem) {}
string Headquarter::Name() { return s(type) + " headquarter"; }

void Headquarter::Create() {
    WarriorType t = prod_seq[type][warr_seq.size() % WARRIOR_NUM];
    if (elem >= str[t]) {
        if (t == DRAGON) warr_seq.push_back(new Dragon(this));
        if (t == NINJA) warr_seq.push_back(new Ninja(this));
        if (t == ICEMAN) warr_seq.push_back(new Iceman(this));
        if (t == LION) warr_seq.push_back(new Lion(this));
        if (t == WOLF) warr_seq.push_back(new Wolf(this));
        warrior[type].insert(*warr_seq.rbegin());
    }
}

bool Headquarter::Conquered() {
    return (warrior[1 - type].size() < 2) ? 0 : printf("%s %s was taken\n", Time().c_str(), Name().c_str());
}

void Headquarter::Report() { printf("%s %d elements in %s\n", Time().c_str(), elem, Name().c_str()); }

Map::Map(int _elem, int _num, vector<int> _str, vector<int> _atk) : num(_num), end(false) {
    Headquarter::beg_elem = _elem;
    city.push_back(new Headquarter(this, RED, 0));
    for (int i = 1; i <= num; ++i) city.push_back(new City(this, i));
    city.push_back(new Headquarter(this, BLUE, num + 1));
    headquarter.push_back((Headquarter *)city[0]);
    headquarter.push_back((Headquarter *)city[num + 1]);
    Headquarter::str.clear();
    Headquarter::atk.clear();
    for (int i : _str) Headquarter::str.push_back(i);
    for (int i : _atk) Headquarter::atk.push_back(i);
}
void Map::Create() {
    for (auto h : headquarter) h->Create();
}
void Map::LionRun() {
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group)
                if (w->type == LION && w->owner->type + c->type != 1) ((Lion *)w)->Run();
}
void Map::Move() {
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group) w->Move();
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group)
                if (w->move) {
                    printf("%s %s %s %s %s\n", Time().c_str(), w->Name().c_str(),
                           (c->isCity() ? "marched to" : "reached"), c->Name().c_str(), w->Info().c_str()),
                        w->move = false;
                    if (!c->isCity()) end |= ((Headquarter *)c)->Conquered();
                }
    // for (auto h : headquarter) end |= h->Conquered();
}
void Map::Update() {
    for (auto c : city)
        if (c->isCity()) c->elem += 10;
}
void Map::Earn() {
    for (auto c : city)
        if (c->isCity()) {
            auto w1 = c->Rank(0), w2 = c->Rank(1);
            if (w2 && !w1) swap(w1, w2);
            if (w1 && !w2) w1->Earn();
        }
}
void Map::UseArrow() {
    for (auto c : city)
        for (int i = 0; i < 2; ++i)
            if (c->type != 1 - i)
                for (auto w1 : c->warrior[i])
                    if (w1->pack[ARROW])
                        for (auto w2 : c->pos(1 - 2 * i)->warrior[1 - i]) w1->Attack(w2, ARROW);
    // Clear();
}
void Map::Clear() {
    for (auto c : city)
        for (auto &group : c->warrior) {
            vector<Warrior *> tmp;
            for (auto w : group)
                if (!w->Alive()) tmp.push_back(w);
            if (tmp.empty()) continue; // debug
            for (auto w : tmp) group.erase(w);
        }
}
void Map::UseBomb() {
    for (auto c : city)
        if (c->isCity()) {
            auto w1 = c->Rank(0), w2 = c->Rank(1);
            if (!(w1 && w2)) continue;
            if (!(w1->Alive() && w2->Alive())) continue;
            if (w2->str <= w1->atk + w1->WeaponDmg()) {
                if (w2->pack[BOMB]) w2->Attack(w1, BOMB);
            } else if (w2->type != NINJA && w1->str <= w2->atk / 2 + w2->WeaponDmg() && w1->pack[BOMB])
                w1->Attack(w2, BOMB);
        }
    // Clear();
}
void Map::Battle() {
    vector<Warrior *> winner[2];
    vector<int> lim;
    for (auto h : headquarter) lim.push_back(h->elem / 8);
    for (auto c : city)
        if (c->isCity()) {
            auto w1 = c->Rank(0), w2 = c->Rank(1);
            if (!(w1 && w2)) continue;
            if (!(w1->Alive() || w2->Alive())) continue;
            if (w1->Alive() && w2->Alive()) {
                w1->tmp_str = w1->str;
                w2->tmp_str = w2->str;
                w1->Attack(w2, SWORD);
                if (w2->Alive()) w2->Counter(w1);
            }
            w1->Res(w2), w2->Res(w1);
            if (w1->Alive() && w1->type == DRAGON) ((Dragon *)w1)->Yell();
            if (w2->Alive() && !w1->Alive()) swap(w1, w2);
            if (w1->Alive() && !w2->Alive()) {
                if (w1->type == WOLF) ((Wolf *)w1)->TakeWeapon(w2);
                winner[w1->owner->type].push_back(w1), w1->Earn(), c->Flag(w1->owner->type);
            } else
                c->Flag();
        }
    reverse(winner[RED].begin(), winner[RED].end());
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < lim[i] && j < winner[i].size(); ++j) winner[i][j]->Gain();
    Clear();
}
void Map::CheckElem() {
    for (auto h : headquarter) h->Report();
}
void Map::CheckWeapon() {
    for (int i = 0; i < 2; ++i)
        for (auto c : city)
            for (auto w : c->warrior[i]) w->Report();
}
void Map::Begin(int t) {
#define mt (global % 60)
    global = 0;
    while (global <= t && !end)
        if (mt == 0)
            Create(), global += 5;
        else if (mt == 5)
            LionRun(), global += 5;
        else if (mt == 10)
            Move(), global += 10;
        else if (mt == 20)
            Update(), global += 10;
        else if (mt == 30)
            Earn(), global += 5;
        else if (mt == 35)
            UseArrow(), global += 3;
        else if (mt == 38)
            UseBomb(), global += 2;
        else if (mt == 40)
            Battle(), global += 10;
        else if (mt == 50)
            CheckElem(), global += 5;
        else if (mt == 55)
            CheckWeapon(), global += 5;
}
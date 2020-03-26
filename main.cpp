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

int main() {
    int test_num = 0;
    cin >> test_num;
    for (int i = 1; i <= test_num; ++i) {
        cout << "Case " << i << ":" << endl;
        int M, N, K, T;
        cin >> M >> N >> K >> T;
        Lion::loyalty_decay = K;
        int str[WARRIOR_NUM];
        int atk[WARRIOR_NUM];
        for (int j = 0; j < WARRIOR_NUM; ++j) cin >> str[j];
        for (int j = 0; j < WARRIOR_NUM; ++j) cin >> atk[j];
        Map a_map(N, M, str, atk);
        for (global_time = 0; global_time <= T;) {
            a_map.red->Create(), a_map.blue->Create();
            if ((global_time += 5) > T) break;
            a_map.CheckLion();
            if ((global_time += 5) > T) break;
            a_map.Move();
            if ((global_time += 25) > T) break;
            a_map.CheckSnatch();
            if ((global_time += 5) > T) break;
            a_map.Battle();
            if ((global_time += 10) > T) break;
            a_map.CheckState();
            if ((global_time += 5) > T) break;
            a_map.CheckWarrior();
            if ((global_time += 5) > T) break;
        }
    }
    return 0;
}

const string Time() {
    char s[7];
    sprintf(s, "%03d:%02d", global_time / 60, global_time % 60);
    return s;
}

const string String(const WeaponType &type) { return weapon_name[type]; }
const string String(const WarriorType &type) { return warrior_name[type]; }
const string String(const HeadquarterType &type) { return headquarter_name[type]; }

Fraction::Fraction(const int &n, const int &d) : numerator(n), denominator(d){};
int operator*(const int &n, const Fraction &f) { return n * f.numerator / f.denominator; }

Weapon::Weapon(const WeaponType &t, const int &p, const int &d) : type(t), damage_multiplier(p), durability(d){};
void Weapon::Wear() { (durability == INF) ? 0 : --durability; }
bool operator<(const Weapon &w1, const Weapon &w2) {
    return (w1.type != w2.type) ? (w1.type < w2.type) : (w1.durability < w2.durability);
}

WeaponPackage::WeaponPackage() : tot(0), cur(0) {}
const Weapon &WeaponPackage::operator[](const int &id) { return weapon[id]; }
bool WeaponPackage::Consumable() {
    for (auto w : weapon)
        if (w.type == BOMB || w.type == ARROW) return true;
    return false;
}
void WeaponPackage::Reset() { cur = 0; }
void WeaponPackage::Scroll() { (cur += 1) %= tot; }
bool WeaponPackage::Empty() { return tot == 0; }
bool WeaponPackage::Full() { return tot == WEAPON_LIMIT; }
void WeaponPackage::Wear() {
    weapon[cur].Wear();
    if (weapon[cur].durability == 0)
        Remove(cur);
    else
        Scroll();
}
void WeaponPackage::Add(const int &n) { weapon[tot++] = Weapon(weapon_template[n % WEAPON_NUM]); }
void WeaponPackage::Add(const Weapon &w) { weapon[tot++] = Weapon(w); }
void WeaponPackage::Remove(const int &id) {
    for (int i = id; i < tot; ++i) weapon[i] = weapon[i + 1];
    weapon[--tot] = Weapon();
    if (cur == tot) cur = 0;
}
void WeaponPackage::Sort() { sort(weapon, weapon + WEAPON_LIMIT); }
WeaponType WeaponPackage::Type(const int &id = -1) { return weapon[(id == -1) ? cur : id].type; }
Fraction WeaponPackage::DamageMultiplier() { return weapon[cur].damage_multiplier; }
const string WeaponPackage::Info() {
    string s;
    int cnt[WEAPON_NUM + 1] = {0, 0, 0};
    for (auto w : weapon) ++cnt[w.type];
    for (int i = 0; i < WEAPON_NUM; ++i) s += to_string(cnt[i]) + " " + String(WeaponType(i)) + " ";
    s.pop_back();
    return s;
}
Warrior::Warrior(Headquarter *o) : alive(true), owner(o) {
    num = o->warrior_cnt + 1;
    pos = (City *)o;
    type = (WarriorType)o->produce_seq[o->cur];
    str = o->warrior_str[type];
    atk = o->warrior_atk[type];
    cout << Time() << " " << Name() << " born" << endl;
}
Warrior::~Warrior() {}
bool Warrior::Alive() { return alive &= (str > 0); }
const string Warrior::Name() { return String(owner->type) + " " + String(type) + " " + to_string(num); }
const string Warrior::Info() {
    return Name() + " marched to " + Pos() + " with " + to_string(str) + " elements and force " + to_string(atk);
}
const string Warrior::Pos() { return "city " + to_string(pos->num); }
int Warrior::Attack(Warrior &w) {
    if (weapon.Empty()) return 0;
    int dmg = atk * weapon.DamageMultiplier();
    w.Hurted(dmg);
    if (weapon.Type() == BOMB) Hurted(dmg / 2);
    weapon.Wear();
    return dmg;
}
void Warrior::Hurted(const int &n) { str -= n; }
void Warrior::Move() {
    City *nxt = (owner->type == RED) ? pos->Next() : pos->Prev();
    nxt->warrior[owner->type] = this;
    pos->warrior[owner->type] = nullptr;
    pos = nxt;
}
pair<int, WeaponType> Warrior::TakeWeapon(Warrior &w) {
    int cnt = 0;
    w.weapon.Sort();
    WeaponType t = w.weapon.Type(0);
    for (int i = w.weapon.tot - 1; i >= 0 && !weapon.Full(); --i)
        if (w.weapon.Type(i) == t) weapon.Add(w.weapon[i]), w.weapon.Remove(i), ++cnt;
    return pair<int, WeaponType>(cnt, t);
}
void Warrior::Take(Warrior &w) {
    while (!(weapon.Full() || w.weapon.Empty())) TakeWeapon(w);
}

Dragon::Dragon(Headquarter *o) : Warrior(o) { weapon.Add(num); }
int Dragon::Attack(Warrior &w) { return Warrior::Attack(w); }
void Dragon::Hurted(const int &n) { Warrior::Hurted(n); }
void Dragon::Take(Warrior &w) { Warrior::Take(w); }
void Dragon::Move() { Warrior::Move(); }
void Dragon::Yell() { cout << Time() << " " << Name() << " yelled in " << Pos() << endl; }

Ninja::Ninja(Headquarter *o) : Warrior(o) { weapon.Add(num), weapon.Add(num + 1); }
int Ninja::Attack(Warrior &w) {
    if (weapon.Empty()) return 0;
    int dmg = atk * weapon.DamageMultiplier();
    w.Hurted(dmg);
    weapon.Wear();
    return dmg;
}
void Ninja::Hurted(const int &n) { Warrior::Hurted(n); }
void Ninja::Take(Warrior &w) { Warrior::Take(w); }
void Ninja::Move() { Warrior::Move(); }

Iceman::Iceman(Headquarter *o) : Warrior(o) { weapon.Add(num); }
int Iceman::Attack(Warrior &w) { return Warrior::Attack(w); }
void Iceman::Hurted(const int &n) { Warrior::Hurted(n); }
void Iceman::Take(Warrior &w) { Warrior::Take(w); }
void Iceman::Move() { Warrior::Move(), str -= str / 10; }

Lion::Lion(Headquarter *o) : Warrior(o) {
    weapon.Add(num);
    loyalty = owner->str;
    cout << Loyalty() << endl;
}
int Lion::Attack(Warrior &w) { return Warrior::Attack(w); }
void Lion::Hurted(const int &n) { Warrior::Hurted(n); }
void Lion::Take(Warrior &w) { Warrior::Take(w); }
void Lion::Move() { Warrior::Move(), loyalty -= loyalty_decay; }
void Lion::TryRun() {
    if (loyalty <= 0) {
        alive = false;
        pos->warrior[owner->type] = nullptr;
        cout << Time() << " " << Name() << " ran away" << endl;
    }
}
const string Lion::Loyalty() { return "Its loyalty is " + to_string(loyalty); }

Wolf::Wolf(Headquarter *o) : Warrior(o) {}
int Wolf::Attack(Warrior &w) { return Warrior::Attack(w); }
void Wolf::Hurted(const int &n) { Warrior::Hurted(n); }
void Wolf::Take(Warrior &w) { Warrior::Take(w); }
void Wolf::Move() { Warrior::Move(); }
void Wolf::Snatch(Warrior &w) {
    if (w.type == WOLF) return;
    pair<int, WeaponType> t = TakeWeapon(w);
    if (t.first)
        cout << Time() << " " << Name() << " took " << t.first << " " << String(t.second) << " from " << w.Name()
             << " in " << Pos() << endl;
}

City::City(Map *p, const int &n) : pos(p), num(n) { memset(warrior, 0, sizeof(warrior)); }
City::~City() {}
City *City::Next() { return pos->city[num + 1]; }
City *City::Prev() { return pos->city[num - 1]; }
void City::Battle() {
    Warrior *w0 = warrior[RED];
    Warrior *w1 = warrior[BLUE];
    if (num % 2 == 0) swap(w0, w1);
    w0->weapon.Sort(), w1->weapon.Sort();
    w0->weapon.Reset(), w1->weapon.Reset();
    for (int round = 0, atk = 1; atk || (w0->weapon.Consumable() || w1->weapon.Consumable()); ++round) {
        atk = 0;
        atk += w0->Attack(*w1);
        if (!(w0->Alive() && w1->Alive())) break;
        atk += w1->Attack(*w0);
        if (!(w0->Alive() && w1->Alive())) break;
    }
    if (num % 2 == 0) swap(w0, w1);
    if (w0->Alive() && w1->Alive())
        cout << Time() << " both " << w0->Name() << " and " << w1->Name() << " were alive in " << w1->Pos() << endl;
    else if (!(w0->Alive() || w1->Alive()))
        cout << Time() << " both " << w0->Name() << " and " << w1->Name() << " died in " << w1->Pos() << endl;
    else {
        if (!w0->Alive()) swap(w0, w1);
        w0->Take(*w1);
        cout << Time() << " " << w0->Name() << " killed " << w1->Name() << " in " << w1->Pos() << " remaining "
             << w0->str << " elements" << endl;
    }
    w0 = warrior[RED];
    w1 = warrior[BLUE];
    if (!w0->Alive())
        warrior[RED] = nullptr;
    else if (w0->type == DRAGON)
        ((Dragon *)w0)->Yell();
    if (!w1->Alive())
        warrior[BLUE] = nullptr;
    else if (w1->type == DRAGON)
        ((Dragon *)w1)->Yell();
}

Headquarter::Headquarter(Map *p, const HeadquarterType &t, const int &n, WarriorType *s)
    : City(p, n), type(t), warrior_cnt(0), cur(0), stop_create(false) {
    memset(warrior_seq, 0, sizeof(warrior_seq));
    str = pos->headquarter_str;
    for (int i = 0; i < WARRIOR_NUM; ++i) {
        produce_seq[i] = s[i];
        warrior_str[i] = pos->str[i];
        warrior_atk[i] = pos->atk[i];
    }
}
Headquarter::~Headquarter() {
    for (auto w : warrior_seq)
        if (w) delete w;
}
const string Headquarter::Name() { return String(type) + " headquarter"; }
void Headquarter::Create() {
    if (str < warrior_str[produce_seq[cur]]) stop_create = true;
    if (stop_create) return;
    WarriorType t = produce_seq[cur];
    str -= warrior_str[t];
    if (t == DRAGON) warrior_seq[warrior_cnt] = new Dragon(this);
    if (t == NINJA) warrior_seq[warrior_cnt] = new Ninja(this);
    if (t == ICEMAN) warrior_seq[warrior_cnt] = new Iceman(this);
    if (t == LION) warrior_seq[warrior_cnt] = new Lion(this);
    if (t == WOLF) warrior_seq[warrior_cnt] = new Wolf(this);
    warrior[type] = warrior_seq[warrior_cnt++];
    (cur += 1) %= WARRIOR_NUM;
}
bool Headquarter::CheckConquered() {
    Warrior *w = warrior[1 - type];
    if (w) {
        cout << Time() << " " << w->Name() << " reached " << Name() << " with " << w->str << " elements and force "
             << w->atk << endl;
        cout << Time() << " " << Name() << " was taken" << endl;
    }
    return w;
}
void Headquarter::CheckState() { cout << Time() << " " << str << " elements in " << Name() << endl; }

Map::Map(const int &n, const int &s, int h[WARRIOR_NUM], int a[WARRIOR_NUM]) : headquarter_str(s) {
    for (int i = 0; i < WARRIOR_NUM; ++i) str[i] = h[i], atk[i] = a[i];
    city_cnt = n;
    city = new City *[n + 2];
    CreateHeadquarter();
}
Map::~Map() {
    for (int i = 0; i <= city_cnt + 1; ++i) delete city[i];
}
void Map::CreateHeadquarter() {
    WarriorType r_seq[] = {ICEMAN, LION, WOLF, NINJA, DRAGON};
    WarriorType b_seq[] = {LION, DRAGON, NINJA, ICEMAN, WOLF};
    for (int i = 1; i <= city_cnt; ++i) city[i] = new City(this, i);
    city[0] = red = new Headquarter(this, RED, 0, r_seq);
    city[city_cnt + 1] = blue = new Headquarter(this, BLUE, city_cnt + 1, b_seq);
}
void Map::Move() {
    for (int i = city_cnt; i >= 0; --i)
        if (city[i]->warrior[RED]) city[i]->warrior[RED]->Move();
    for (int i = 1; i <= city_cnt + 1; ++i)
        if (city[i]->warrior[BLUE]) city[i]->warrior[BLUE]->Move();
    bool conquered = false;
    conquered |= red->CheckConquered();
    for (int i = 1; i <= city_cnt; ++i)
        for (int j = 0; j < HEADQUARTER_NUM; ++j)
            if (city[i]->warrior[j]) cout << Time() << " " << city[i]->warrior[j]->Info() << endl;
    conquered |= blue->CheckConquered();
    if (conquered) global_time = 0x3fffffff;
}
void Map::CheckState() { red->CheckState(), blue->CheckState(); }
void Map::CheckWarrior() {
    for (int i = 0; i <= city_cnt + 1; ++i)
        for (int j = 0; j < HEADQUARTER_NUM; ++j) {
            Warrior *w = city[i]->warrior[j];
            if (w)
                cout << Time() << " " << w->Name() << " has " << w->weapon.Info() << " and " << w->str << " elements"
                     << endl;
        }
}
void Map::CheckLion() {
    for (int i = 0; i <= city_cnt + 1; ++i)
        for (int j = 0; j < HEADQUARTER_NUM; ++j) {
            Warrior *w = city[i]->warrior[j];
            if (w && w->type == LION) ((Lion *)w)->TryRun();
        }
}
void Map::CheckSnatch() {
    for (int i = 0; i <= city_cnt + 1; ++i) {
        Warrior *w0 = city[i]->warrior[RED];
        Warrior *w1 = city[i]->warrior[BLUE];
        if (w0 && w1) {
            if (w0->type == WOLF) ((Wolf *)w0)->Snatch(*w1);
            if (w1->type == WOLF) ((Wolf *)w1)->Snatch(*w0);
        }
    }
}
void Map::Battle() {
    for (int i = 0; i <= city_cnt + 1; ++i) {
        Warrior *w0 = city[i]->warrior[RED];
        Warrior *w1 = city[i]->warrior[BLUE];
        if (w0 && w1) city[i]->Battle();
    }
}
#include "header/MyClass.hpp"

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
string Weapon::Info() { return s(type) + "(" + to_string(type == SWORD ? dmg : dura) + ")"; }
bool Weapon::Broken() { return dura == 0 || dmg == 0; }
Sword::Sword(int dmg) : Weapon(SWORD, dmg, -1) {}
void Sword::Wear() { dmg = dmg * 4 / 5; }
Bomb::Bomb(int dmg) : Weapon(BOMB, dmg, 1) {}
Arrow::Arrow(int dmg) : Weapon(ARROW, dmg, 3) {}

int Warrior::WeaponDmg() { return pack[SWORD] ? 0 : pack[SWORD]->dmg; }
int Warrior::UseWeapon(WeaponType w) {
    if (!pack[w]) return 0;
    int d = pack[w]->Use();
    if (pack[w]->Broken()) delete pack[w], pack[w] = nullptr;
    return d;
}
Warrior::Warrior(WarriorType t, Headquarter *_owner) {}
Warrior::Warrior(WarriorType t, Headquarter *_owner) : type(t), move(false), owner(_owner) {
    num = owner->warr_seq.size() + 1;
    Gain(owner->str[t]);
    atk = owner->atk[t];
    city = owner;
    printf("%s %s born\n", Time().c_str(), Name().c_str());
}
bool Warrior::Alive() { return str; }
void Warrior::Dead(Warrior *w, WeaponType t) {
    if (t == SWORD) printf("%s %s was killed in %s\n", Time().c_str(), Name().c_str(), city->Name().c_str());
    if (t == BOMB) printf("%s %s used a bomb and killed %s\n", Time().c_str(), w->Name().c_str(), Name().c_str());
    if (t == ARROW) printf("%s %s shot and killed %s\n", Time().c_str(), w->Name().c_str(), Name().c_str());
}
string Warrior::Name() { return s(owner->type) + " " + s(type) + " " + to_string(num); }
string Warrior::Info() { return "with " + to_string(str) + " elements and force " + to_string(atk); }
void Warrior::Arm(WeaponType t) {
    if (t == SWORD) pack[t] = new Sword(atk / 5);
    if (t == BOMB) pack[t] = new Bomb();
    if (t == ARROW) pack[t] = new Arrow();
}
void Warrior::Attack(Warrior *w, WeaponType t) {
    if (t == SWORD)
        w->Hurt(atk + UseWeapon(t)), printf("%s %s attacked %s in %s %s\n", Time().c_str(), Name().c_str(),
                                            w->Name().c_str(), city->Name().c_str(), Info().c_str());
    if (t == BOMB) UseWeapon(t), Hurt(str), w->Hurt(w->str);
    if (t == ARROW) w->Hurt(UseWeapon(t));
    if (!w->Alive()) w->Dead(this, t);
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
        if (w) info += w->Info(), info += ",";
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
void Lion::Dead(Warrior *w) { Warrior::Dead(w, SWORD), w->Hurt(-tmp_str); }
void Lion::Run() {
    if (loyalty <= 0) printf("%s %s ran away\n", Time().c_str(), Name().c_str());
    city->warrior[type].erase(this);
}

Wolf::Wolf(Headquarter *_owner) : Warrior(WOLF, _owner) {}
void Wolf::TakeWeapon(Warrior *w) {
    for (int i = 0; i < WEAPON_NUM; ++i)
        if (!pack[i]) swap(pack[i], w->pack[i]);
}

Warrior *City::Rank(int n) {
    bool rank = (n & 1) ^ (flag == RED || (flag == UNDEFINED && num % 2 == 1));
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

Map::Map(int _elem, int _num, vector<int> _str, vector<int> _atk) : num(_num) {
    Headquarter::beg_elem = _elem;
    city.push_back(new Headquarter(this, RED, 0));
    for (int i = 1; i <= num; ++i) city.push_back(new City(this, i));
    city.push_back(new Headquarter(this, BLUE, num + 1));
    headquarter.push_back((Headquarter *)city[0]);
    headquarter.push_back((Headquarter *)city[num + 1]);
    for (int i : _str) Headquarter::str.push_back(i);
    for (int i : _atk) Headquarter::atk.push_back(i);
}
void Map::Create() {
    for (auto h : headquarter) h->Create();
}
void Map::LionRun() {
    for (int i = 1; i <= num; ++i)
        for (auto group : city[i]->warrior)
            for (auto w : group)
                if (w->type == LION) ((Lion *)w)->Run();
}
void Map::Move() {
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group) w->Move();
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group)
                if (w->move)
                    printf("%s %s marched to %s %s\n", Time().c_str(), w->Name().c_str(), c->Name().c_str(),
                           w->Info().c_str()),
                        w->move = false;
    for (auto h : headquarter) end ^= h->Conquered();
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
                        for (auto w2 : c->pos(2 * i - 1)->warrior[1 - i]) w1->Attack(w2, ARROW);
    Clear();
}
void Map::Clear() {
    for (auto c : city)
        for (auto group : c->warrior)
            for (auto w : group)
                if (!w->Alive()) group.erase(w);
}
void Map::UseBomb() {
    for (auto c : city)
        if (c->isCity()) {
            auto w1 = c->Rank(0), w2 = c->Rank(1);
            if (!(w1 && w2)) continue;
            if (w2->str <= w1->atk + w1->WeaponDmg()) {
                if (w2->pack[BOMB]) w2->Attack(w1, BOMB);
            } else if (w1->str <= w2->atk / 2 + w2->WeaponDmg() && w1->pack[BOMB])
                w1->Attack(w2, BOMB);
        }
    Clear();
}
void Map::Battle() {
    vector<Warrior *> winner[2];
    vector<int> lim;
    for (auto h : headquarter) lim.push_back(h->elem / 8);
    for (auto c : city)
        if (c->isCity()) {
            auto w1 = c->Rank(0), w2 = c->Rank(1);
            if (!(w1 && w2)) continue;
            w1->tmp_str = w1->str;
            w2->tmp_str = w2->str;
            w1->Attack(w2, SWORD);
            if (w2->Alive()) w2->Counter(w1);
            w1->Res(w2), w2->Res(w1);
            if (w1->Alive() && w1->type == DRAGON) ((Dragon *)w1)->Yell();
            if (w2->Alive() && !w1->Alive()) swap(w1, w2);
            if (w1->Alive() && !w2->Alive())
                winner[w1->type].push_back(w1), w1->Earn(), c->Flag(w1->owner->type);
            else
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
    while (global <= t && !end) {
        int min = global % 60;
        if (min == 0)
            Create(), global += 5;
        else if (min == 5)
            LionRun(), global += 5;
        else if (min == 10)
            Move(), global += 10;
        else if (min == 20)
            Update(), global += 10;
        else if (min == 30)
            Earn(), global += 5;
        else if (min == 35)
            UseArrow(), global += 3;
        else if (min == 38)
            UseBomb(), global += 2;
        else if (min == 40)
            Battle(), global += 10;
        else if (min == 50)
            CheckElem(), global += 5;
        else if (min == 55)
            CheckWeapon(), global += 5;
    }
}
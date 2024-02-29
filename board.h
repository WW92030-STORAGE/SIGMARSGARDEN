#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <stack>

#include "symbol.h"

class Board {
    public:
    int radius; // How many tiles are on each side of the hexagon?
    std::map<Triple, Point> tiles;
    std::set<Triple> taken;
    std::stack<std::pair<Triple, Triple>> order; // Order of moves. Gold is taken by itself so its entry has two identical coords.
    int metalcount = 0;
    Color metals[6] = {LEAD, TIN, IRON, COPPER, SILVER, GOLD};
    
    // 0 = directly right
    
    int dx[6] = {01, 01, 00, -1, -1, 00};
    int dy[6] = {-1, 00, 01, 01, 00, -1};
    int dz[6] = {00, -1, -1, 00, 01, 01};
    
    Board() {
        radius = 5;
    }
    
    Board(int r) {
        radius = r;
    }
    
    Board(Board& b) {
        radius = b.radius;
        for (auto i : b.tiles) tiles.insert({i.first, i.second});
    }
    
    bool isEmpty(Triple test) {
        return (tiles.find(test) == tiles.end()) || (tiles.at(test).element == NIL) || (taken.find(test) != taken.end());
    }
    
    int CountEmptySpaces(Triple p) {
        int cnt = 0;
        int res = 0;
        for (int i = 0; i < 12; i++) {
            Triple test(p.x + dx[i % 6], p.y + dy[i % 6], p.z + dz[i % 6]);
            if (!isEmpty(test)) {
                cnt = 0;
                continue;
            }
            if (cnt < 6) cnt++;
            res = max(res, cnt);
        }
        return res;
    }
    
    std::vector<Triple> getFreeSpaces(std::set<Triple> s = {}, bool EMPTY_ONLY = true, bool OCCUPIED_ONLY = false, bool MUST_TOUCH = true) {
        
        std::vector<Triple> v;
        for (int layer = 0; layer <= radius; layer++) {
            Triple pos(-1 * layer, 0, layer);
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < max(layer, 1); j++) {
                    if (EMPTY_ONLY && !isEmpty(pos)) {
                        pos = pos + Triple(dx[i], dy[i], dz[i]);
                        continue;
                    }
                    if (OCCUPIED_ONLY && isEmpty(pos)) {
                        pos = pos + Triple(dx[i], dy[i], dz[i]);
                        continue;
                    }
                    if (s.size() > 0 && s.find(pos) == s.end()) {
                        pos = pos + Triple(dx[i], dy[i], dz[i]);
                        continue;
                    }
                    // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
                    if (CountEmptySpaces(pos) >= 3) {
                        if (!MUST_TOUCH || (CountEmptySpaces(pos) < 6)) {
                            // std::cout << "FREE\n";
                            v.push_back(pos);
                        }
                    }
                    pos = pos + Triple(dx[i], dy[i], dz[i]);
                }
            }
            pos = pos + Triple(-1, 0, 1);
        }
        return v;
    }
    
    void place(Triple x, Color e) {
        if (!isEmpty(x)) {
            // cout << "ERROR - " << x.x << " " << x.y << " " << x.z << " NOT EMPTY\n";
        }
        Point p(x, e);
        tiles.insert({x, p});
    }
    
    std::vector<std::pair<Color, Color>> getRandomColorList(int elements = 8, int salts = 4, int vim = 4) {
        std::vector<std::pair<Color, Color>> thelist;
        
        for (int i = 0; i < elements / 2; i++) {
            thelist.push_back({FIRE, FIRE});
            thelist.push_back({WATER, WATER});
            thelist.push_back({EARTH, EARTH});
            thelist.push_back({AIR, AIR});
        }
        
        for (int i = 0; i < salts / 2; i++) thelist.push_back({SALT, SALT});
        for (int i = 0; i < vim; i++) thelist.push_back({VITAE, MORS});
        for (int i = 0; i < 5; i++) thelist.push_back({METAL, MERCURY}); // Five metals
        random_shuffle(thelist.begin(), thelist.end());
        return thelist;
    }
    
    bool generate(std::set<Triple> layout = {}, int attempts = 64) {
        if ((attempts <= 0) && (layout.size() > 0)) {
            cout << "FAILSAFE OCCURRED!\n";
            generate({}, 0);
            return false;
        }
        tiles.clear();
        taken.clear();
        place(Triple(0, 0, 0), GOLD);
        std::vector<std::pair<Color, Color>> gen = getRandomColorList();
        // cout << gen.size() << " TILE PAIRS TO PLACE\n";
        
        int metalcount = 0;
        Color metals[5] = {SILVER, COPPER, IRON, TIN, LEAD};
        
        for (auto i : gen) {
            std::vector<Triple> freespaces = getFreeSpaces(layout, true, false);
            if (freespaces.size() == 0) {
                // cout << "ERROR NO MORE FREE SPACES " << attempts << " " << layout.size() << "\n";
                break;
            }
            std::pair<int, int> places = randpair(freespaces.size());
            if (i.first != METAL) place(freespaces[places.first], i.first);
            else place(freespaces[places.first], metals[metalcount++]);
            if (i.second != METAL) place(freespaces[places.second], i.second);
            else place(freespaces[places.second], metals[metalcount++]);
        }
        
        if (tiles.size() != gen.size() * 2 + 1) {
            if (attempts <= 0) return false;
            if (attempts % 1 == 0) return generate(generateLayout(), attempts - 1);
            else return generate(layout, attempts - 1);
        }
        return true;
    }
    
    void tilelist() {
        for (auto x : tiles) {
            Triple pos = x.first;
            std::cout << pos.x << " " << pos.y << " " << pos.z << " = " << x.second.element << std::endl;
        }
        std::cout << tiles.size() << " TILES\n";
    }
    
    void disp() {
        for (int i = -1 * radius; i <= radius; i++) {
            for (int j = 0; j < abs(i); j++) std::cout << " ";
            int x = max(-1 * (radius + i), -1 * radius);
            int y = min(radius, radius - i);
            while (x <= y) {
                Triple pos = {x, (-1 * i) - x, i};
                // std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
                if (!isEmpty(pos) && taken.find(pos) == taken.end()) std::cout << getsymbol(tiles.at(pos).element) << " ";
                else if (taken.find(pos) == taken.end()) std::cout << ". ";
                else std::cout << "/ ";
                x++;
            }
            std::cout << "\n";
        }
    }
    
    std::set<Triple> generateLayout(int tiling = 55) {
        std::set<Triple> res;
        res.insert({0, 0, 0});
        place({0, 0, 0}, GOLD);
        for (int i = 0; i < tiling / 3; i++) {
            std::vector<Triple> freespaces = getFreeSpaces({}, true, false);
            if (freespaces.size() == 0) {
                // cout << "ERROR NO MORE FREE SPACES TO CHOOSE\n";
                break;
            }
            int base = randint(freespaces.size());
            res.insert(freespaces[base]);
            res.insert(freespaces[base].rot());
            res.insert(freespaces[base].rot().rot());
            place(freespaces[base], GOLD);
            place(freespaces[base].rot(), GOLD);
            place(freespaces[base].rot().rot(), GOLD);
        }
        tiles.clear();
        return res;
    }
    
    bool isMetal(Color a) {
        for (int i = 0; i < 6; i++) {
            if (a == metals[i]) return true;
        }
        return false;
    }
    
    bool isCardinal(Color a) {
        return a == AIR || a == FIRE || a == EARTH || a == WATER;
    }
    
    bool compatible(Color a, Color b) {
        if (a == SALT) return b == SALT || isCardinal(b);
        if (b == SALT) return a == SALT || isCardinal(a);
        if (isCardinal(a)) return b == a || b == SALT;
        if (isCardinal(b)) return a == b || a == SALT;
        if (a == VITAE) return b == MORS;
        if (b == VITAE) return a == MORS;
        
        if (a == GOLD && b == GOLD && metalcount == 5) return true;
        
        if (a == MERCURY) return b == metals[metalcount];
        if (b == MERCURY) return a == metals[metalcount];
        return false;
    }
    
    bool solvable() {
        int saltcount = 0;
        int blah[4] = {0, 0, 0, 0};
        for (auto i : tiles) {
            if (isEmpty(i.first)) continue;
            switch (i.second.element) {
                case AIR:
                blah[0]++;
                break;
                case WATER:
                blah[1]++;
                break;
                case EARTH:
                blah[2]++;
                break;
                case FIRE:
                blah[3]++;
                break;
                case SALT:
                saltcount++;
                break;
            }
        }
        
        for (int i = 0; i < 4; i++) {
            if (blah[i] % 2 != 0) saltcount--;
        }
        return saltcount >= 0;
    }
    
    bool move(Triple a, Triple b) {
        if (a == b) return false;
        if (taken.find(a) != taken.end()) return false;
        if (taken.find(b) != taken.end()) return false;
        if (isEmpty(a) || isEmpty(b)) return false;
        if (CountEmptySpaces(a) < 3 || CountEmptySpaces(b) < 3) return false;
        
        Point p1 = tiles.at(a);
        Point p2 = tiles.at(b);
        
        if (!compatible(p1.element, p2.element)) return false;
        
        order.push({p1.pos, p2.pos});
        if (isMetal(p1.element) || isMetal(p2.element)) metalcount++;
        
        taken.insert(a);
        taken.insert(b);
        
        // std::cout << "MOVE SUCCESSFUL\n";
        
        return true;
    }
    
    void undo() {
        if (order.size() <= 0) return;
        
        std::pair<Triple, Triple> bling = order.top();
        order.pop();
        
        Point p1 = tiles.at(bling.first);
        Point p2 = tiles.at(bling.second);
        if (isMetal(p1.element) || isMetal(p2.element)) metalcount--;
        
        if (taken.find(bling.first) != taken.end()) taken.erase(taken.find(bling.first));
        if (taken.find(bling.second) != taken.end()) taken.erase(taken.find(bling.second));
    }
};

#endif

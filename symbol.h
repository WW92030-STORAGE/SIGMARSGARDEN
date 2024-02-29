#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

enum Color { // Color is an enum to prevent exploiting loopholes (e.g. 2 similar colors considered different)
    NIL = INT_MAX, // Empty color
    INVALID = INT_MIN, // Invalid space
    
    FIRE = 0xFF0000, // RED
    EARTH = 0x00FF00, // GREEN
    WATER = 0x0000FF, // WATER
    AIR = 0x00FFFF, // CYAN
    
    MORS = 0, // BLACK
    VITAE = 0xFFFFFF, // WHITE
    
    
    SALT = 0xC0C0C0, // LIGHT
    
    
    GOLD = 0xFFFF00, // YELLOW
    SILVER = 0x808080, // GREY
    COPPER = 0xFF8000, // ORANGE
    IRON = 0xFFC0CB, // HTML PINK
    TIN = 0xEEE8AA, // HTML PALE GOLDENROD
    LEAD = 0x404040, // DARK
    METAL = 0x010101,
    
    MERCURY = 0xFFE4E1, // HTML MISTYROSE
    
    
    // Furries!
    
    SP2 = 0xCEB7FF,
    SP1 = 0x8AD8FF
};

std::string getsymbol(Color e) {
    switch (e) {
        case NIL:
        return ".";
        case INVALID:
        return "Χ";
        case AIR:
        return "Α";
        case FIRE:
        return "Δ";
        case EARTH:
        return "Ɐ";
        case WATER:
        return "∇";
        case MORS:
        return "-";
        case VITAE:
        return "+";
        
        case SALT:
        return "Θ";
        case MERCURY:
        return "☿";
        case LEAD:
        return "♄";
        case TIN:
        return "♃";
        case IRON:
        return "♂";
        case COPPER:
        return "♀";
        case SILVER:
        return "☽";
        case GOLD:
        return "☉";
        case METAL:
        return "#";
    }
    return "?";
}

class Triple {
    public:
    int x, y, z;
    
    Triple() {
        x = 0;
        y = 0;
        z = 0;
    }
    
    Triple(int a, int b, int c) {
        x = a;
        y = b;
        z = c;
    }
    
    bool operator<(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a < b;
    }
    
    bool operator>(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a > b;
    }
    
    bool operator<=(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a <= b;
    }
    
    bool operator>=(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a >= b;
    }
    
    bool operator==(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a == b;
    }
    
    bool operator!=(const Triple& obj) const {
        std::tuple<int, int, int> a = {x, y, z};
        std::tuple<int, int, int> b = {obj.x, obj.y, obj.z}; 
        return a != b;
    }
    
    Triple operator+(const Triple& obj) const {
        return Triple(x + obj.x, y + obj.y, z + obj.z);
    }
    
    Triple operator-(const Triple& obj) const {
        return Triple(x - obj.x, y - obj.y, z - obj.z);
    }
    
    Triple rot() {
        return Triple(y, z, x);
    }
    
    Triple clone() {
        return Triple(x, y, z);
    }
};

class Point {
    public:
    Triple pos;
    Color element;
    
    vector<Color> ELEMENTS = {NIL, INVALID, AIR, FIRE, EARTH, WATER, MORS, VITAE, SALT, MERCURY, LEAD, TIN, IRON, COPPER, SILVER, GOLD};
    
    Point() {
        pos = Triple();
        element = NIL;
    }
    
    Point(Triple t, Color e) {
        pos = t;
        element = e;
    }
    
    Point clone() {
        return Point(pos.clone(), element);
    }
    
    bool operator<(const Point& obj) const {
        if (obj.pos != pos) return pos < obj.pos;
        int a1 = 0;
        int a2 = 0;
        for (int i = 0; i < ELEMENTS.size(); i++) {
            if (element == ELEMENTS[i]) a1 = i;
            if (obj.element == ELEMENTS[i]) a2 = i;
        }
        return a1 < a2;
    }
    
    bool operator==(const Point& obj) const {
        return obj.pos == pos && obj.element == element;
    }
};

int randint(int upper) { // Change this as needed
    if (upper <= 0) return 0;
    return rand() % upper;
}

std::pair<int, int> randpair(int upper, int attempts = 32) {
    std::pair<int, int> res = {0, 0};
    if (upper <= 0) return {-1, -1};
    for (int i = 0; i < attempts; i++) {
        res.first = randint(upper);
        res.second = randint(upper);
        if (res.first != res.second) return res;
    }
    
    if (res.first == res.second) res.second = (res.second + 1) % upper;
    return res;
}

#endif

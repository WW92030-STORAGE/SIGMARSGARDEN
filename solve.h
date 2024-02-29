#ifndef SOLVE_H
#define SOLVE_H

#include "board.h"
#include "symbol.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <stack>

class Solver {
    public:
    Board b;
    
    std::vector<std::pair<Triple, Triple>> resultant;
    int numchecked;
    bool contd;
    
    Solver() {
        b = Board();
    }
    
    Solver(Board& board) {
        b = board;
        b.taken.clear();
        b.order = std::stack<std::pair<Triple, Triple>>();
    }
    
    void reset(Board& board) {
        b = board;
        b.taken.clear();
        b.order = std::stack<std::pair<Triple, Triple>>();
    }
    
    void constructsolution() {
        std::cout << b.order.size() << std::endl;
        while (b.order.size() > 0) {
            resultant.push_back(b.order.top());
            b.order.pop();
        }
        std::reverse(resultant.begin(), resultant.end());
    }
    
    void solve(bool starting = true) {
        if (starting) {
            contd = true;
            numchecked = 0;
        }
        numchecked++;
        if (!contd) {
            std::cout << "END\n";
            return;
        }
        std::vector<Triple> spots = b.getFreeSpaces({}, false, true, false);
        // for (auto i : spots) std::cout << i.x << " " << i.y << " " << i.z << "\n";
        
        if (numchecked % 1024 == 0) {
            std::cout << b.taken.size() << " TILES FREED\n";
        std::cout << spots.size() << " TILES UNBLOCKED\n";
        std::cout << numchecked << " STATES ANALYZED\n";
        std::cout << b.metalcount << " METALS COLLECTED\n";
        }
        
        if (b.taken.size() == b.tiles.size()) {
            contd = false;
            numchecked = 0;
                std::cout << "SOLVED\n";
                constructsolution();
                return;
        }
        
        if (spots.size() == 1) {
            numchecked = 0;
            if (b.tiles.at(spots[0]).element == GOLD) {
                contd = false;
                std::cout << "SOLVED\n";
                constructsolution();
                return;
            }
            std::cout << "UNSUCCESSFUL\n";
            return;
        }
        
        if (spots.size() == 0) {
            return;
        }
        
        std::random_shuffle(spots.begin(), spots.end());
        
        for (int i = 0; i < spots.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (b.move(spots[i], spots[j])) {
                    // b.disp();
                    if (!b.solvable()) {
                        // std::cout << "BAD BAD BAD BAD BAD BAD\n";
                        b.undo();
                        continue;
                    }
                    solve(false);
                    if (!contd) return;
                    b.undo();
                }
            }
        }
    }
    
    void dispsolution() {
        for (auto i : resultant) {
            std::cout << i.first.x << " " << i.first.y << " " << i.first.z << " | " << getsymbol(b.tiles.at(i.first).element);
            std::cout << "\n";
            std::cout << i.second.x << " " << i.second.y << " " << i.second.z << " | " << getsymbol(b.tiles.at(i.second).element);
            std::cout << "\n\n";
        }
    }
};

#endif

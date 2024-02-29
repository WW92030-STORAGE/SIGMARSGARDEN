#include <bits/stdc++.h>
using namespace std;

#include "board.h"
#include "solve.h"

int main()
{
    srand(time(0));
    Board b(5);
    
    b.place({0, 0, 0}, AIR);
    b.place({0, 1, -1}, AIR);
    b.place({1, -1, 0}, FIRE);
    b.place({-1, 0, 1}, FIRE);
    
    auto v = b.generateLayout(55);
        b.generate(v);
    
    b.disp();
    
    Solver s(b);
    s.solve();
    cout << "terminated\n";
    
    s.dispsolution();
    
    return 0;
}

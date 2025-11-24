#pragma once
#include "core/Grid.h"

class Simulator {
private:
    // double buffering impl
    Grid curr_grid;
    Grid next_grid;

    // helper: count living neighbors around (x,y)
    int count_neighbors(int x, int y) const;


public:
    Simulator(int width, int height);

    // advance 1 generation
    void step();

    // access to current grid for rendering/querying
    const Grid& get_grid() const;

    // way to set initial patterns
    void set_cell(int x, int y, bool alive);    // modify init state
};

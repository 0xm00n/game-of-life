#pragma once

#include <vector>
#include <cstdint>

class Grid {
private:
    // member vars
    // width and height of grid (int for simplicity, size_t to be more precise)
    int width;
    int height;
    
    // cells
    std::vector<uint8_t> cells;

    // helper to convert 2D to 1D index
    int get_index(int x, int y) const;

public:
    Grid(int width, int height);
    bool is_alive(int x, int y) const;
    void set_cell(int x, int y, bool alive);
    void clear();

    int get_width() const { return width; }
    int get_height() const { return height; }

};

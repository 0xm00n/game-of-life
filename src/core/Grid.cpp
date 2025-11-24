#include "core/Grid.h"
#include <algorithm>


Grid::Grid(int width, int height)
    : width(width), height(height), cells(width * height, 0) {}

int Grid::get_index(int x, int y) const {
    return (y * width) + x;
}

bool Grid::is_alive(int x, int y) const {
    return cells[get_index(x, y)];
}   

void Grid::set_cell(int x, int y, bool alive) {
    cells[get_index(x, y)] = alive ? 1 : 0;
}

void Grid::clear() {
    std::fill(cells.begin(), cells.end(), 0);
}

#include "simulation/Simulator.h"

Simulator::Simulator(int width, int height)
    : curr_grid(width, height), next_grid(width, height) {}

const Grid& Simulator::get_grid() const {
    return curr_grid;
}

void Simulator::set_cell(int x, int y, bool alive) {
    curr_grid.set_cell(x, y, alive);
}

// Moore neighborhood
int Simulator::count_neighbors(int x, int y) const {
    int count = 0;

    // since we store the 2D grid as index = y * width + x, we have y as outer loop for cache locality
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <=1; dx++) {
            if (dy == 0 && dx == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < curr_grid.get_width() && ny >= 0 && ny < curr_grid.get_height()) {
                if (curr_grid.is_alive(nx, ny)) {
                    count++;
                }
            }
        }
    }

    return count;
}

void Simulator::step() {
    for (int y = 0; y < curr_grid.get_height(); y++) {
        for (int x = 0; x < curr_grid.get_width(); x++) {
            int living_neighbors = count_neighbors(x, y);
            bool is_curr_alive = curr_grid.is_alive(x, y);
            bool will_be_alive = false;

            // dont need to explicitly code the dead cell conditionals since will_be_alive is set to false on default
            if (is_curr_alive && (living_neighbors == 2 || living_neighbors == 3)) {
                will_be_alive = true;
            } else if (!is_curr_alive && living_neighbors == 3) {
                will_be_alive = true;
            }

            next_grid.set_cell(x, y, will_be_alive);
        }
    }

    std::swap(curr_grid, next_grid);
}

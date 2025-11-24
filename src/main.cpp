#include "simulation/Simulator.h"
#include <iostream>

void print_grid(const Grid& grid) {
    for (int y = 0; y < grid.get_height(); y++) {
        for (int x = 0; x < grid.get_width(); x++) {
            std::cout << (grid.is_alive(x, y) ? "◼" : "◻");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    Simulator sim = Simulator(20, 20);

    sim.set_cell(6, 5, true);   // . ◼ .
    sim.set_cell(7, 6, true);   // . . ◼
    sim.set_cell(5, 7, true);   // ◼ ◼ ◼
    sim.set_cell(6, 7, true);
    sim.set_cell(7, 7, true);

    std::cout << "Generation 0:\n";
    print_grid(sim.get_grid());

    // Run and print every 4 generations
    for (int gen = 1; gen <= 20; gen++) {
        sim.step();
        if (gen % 4 == 0) {
            std::cout << "\nGeneration " << gen << ":\n";
            print_grid(sim.get_grid());
        }
    }

    return 0;
}

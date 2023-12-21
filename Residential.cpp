#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "residential.h"

void Residential::residentialGrowth(char** grid, int** populations, int citySize) {
    for (int y = 0; y < citySize; y++) {
        for (int x = 0; x < citySize; x++) {
            if (grid[y][x] == 'R') {  // Check if the cell is a ResidentialZone
                int adjacentPopulation = countAdjacentPopulation(populations, citySize, x, y);
                if (adjacentPopulation >= 3) {
                    populations[y][x]++;  // Increase the population if the conditions are met
                }
            }
        }
    }
}

int Residential::countAdjacentPopulation(int** populations, int citySize, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) {
                continue;  // Skip the current cell
            }
            int neighborX = x + dx;
            int neighborY = y + dy;
            if (neighborX >= 0 && neighborX < citySize && neighborY >= 0 && neighborY < citySize) {
                count += populations[neighborY][neighborX];  // Add the population of the adjacent cell
            }
        }
    }
    return count;
}


#ifndef INDUSTRIAL_H
#define INDUSTRIAL_H

class Industrial {
public:
    void industrialGrowth(char grid, int populations, int citySize);

private:
    int countAdjacentPopulation(int** populations, int citySize, int x, int y);
    bool hasEnoughAvailableWorkers(int availableWorkers);
};

#endif
#pragma once

#ifndef RESIDENTIAL_H
#define RESIDENTIAL_H

class Residential {
public:
    void residentialGrowth(char** grid, int** populations, int citySize);

private:
    int countAdjacentPopulation(int** populations, int citySize, int x, int y);
};

#endif


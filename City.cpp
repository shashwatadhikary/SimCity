#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include "residential.h"  // Include the residential header
#include "industrial.h"// Include the industrial header 

using namespace std;

const char Road = '-';
const char PowerLine = 'T';
const char PowerLineRoad = '#';
const char ResidentialZone = 'R';
const char IndustrialZone = 'I';
const char CommercialZone = 'C';
const char EMPTY = '.';
const char BUILDING = 'B';
const char CITIZEN = 'P';

class City {
public:
    char** grid;
    int** populations;
    char** zones;
    int citySize;
    int numCitizens;

public:
    City(int size) : citySize(size), numCitizens(0) {
        grid = new char* [citySize];
        populations = new int* [citySize];
        zones = new char* [citySize];

        for (int i = 0; i < citySize; i++) {
            grid[i] = new char[citySize];
            populations[i] = new int[citySize];
            zones[i] = new char[citySize];

            for (int j = 0; j < citySize; j++) {
                grid[i][j] = Road;
                populations[i][j] = 0;
                zones[i][j] = Road;
            }
        }
    }

    char** getGrid() const {
        return grid;
    }

    int** getPopulations() const {
        return populations;
    }

    char** getZones() const {
        return zones;
    }

    int getCitySize() const {
        return citySize;
    }

    void build(int x, int y) {
        if (x >= 0 && x < citySize && y >= 0 && y < citySize) {
            grid[y][x] = PowerLineRoad;
        }
    }

    void spawnCitizen(int x, int y) {
        if (x >= 0 && x < citySize && y >= 0 && y < citySize && grid[y][x] == Road) {
            grid[y][x] = ResidentialZone;
            populations[y][x]++;
            numCitizens++;
        }
    }

    int getNumCitizens() const {
        return numCitizens;
    };

    void displayCity() {
        for (int y = 0; y < citySize; y++) {
            for (int x = 0; x < citySize; x++) {
                if (grid[y][x] == Road) {
                    cout << zones[y][x];
                }
                else {
                    cout << populations[y][x];
                }
                cout << ' ';
            }
            cout << endl;
        }
    }

    // Growth functions
    bool canGrow(int x, int y) {
        // Check if the cell is empty and has adjacent cells with populations
        if (grid[y][x] == Road) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (x + dx >= 0 && x + dx < citySize && y + dy >= 0 && y + dy < citySize) {
                        if (populations[y + dy][x + dx] > 0) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    int calculateTotalAdjacentPopulations(int x, int y) {
        int totalAdjacentPopulations = 0;

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (x + dx >= 0 && x + dx < citySize && y + dy >= 0 && y + dy < citySize) {
                    totalAdjacentPopulations += populations[y + dy][x + dx];
                }
            }
        }

        return totalAdjacentPopulations;
    }

    bool shouldGrowCommercial(int x, int y, int totalAdjacentPopulations) {
        // Check if it's a commercial zone and has enough adjacent populations
        return (zones[y][x] == CommercialZone && totalAdjacentPopulations >= 1);
    }

    void growCommercial(int x, int y) {
        // Set the cell as a citizen and increment the population
        grid[y][x] = CITIZEN;
        populations[y][x]++;
        numCitizens++;
    }

    bool shouldGrowIndustrial(int x, int y, int totalAdjacentPopulations) {
        // Check if it's an industrial zone and has enough adjacent populations
        return (zones[y][x] == IndustrialZone && totalAdjacentPopulations >= 2);
    }

    void growIndustrial(int x, int y) {
        // Set the cell as a citizen and increment the population
        grid[y][x] = CITIZEN;
        populations[y][x]++;
        numCitizens++;
    }




    ~City() {
        for (int i = 0; i < citySize; i++) {
            delete[] grid[i];
            delete[] populations[i];
            delete[] zones[i];
        }
        delete[] grid;
        delete[] populations;
        delete[] zones;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    int defaultCitySize = 20;
    int citySize = 0;
    int numInitialCitizens = 0;
    int timeLimit = 20;
    int refreshRate = 1;
    int numBuildings = 0;
    Residential residential; // Create an instance of the Residential class
    Industrial industrial;



    ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        cerr << "Error: Could not open configuration file." << endl;
        return 1;
    }

    string line;
    while (getline(configFile, line)) {
        if (line.find("CitySize=") == 0) {
            citySize = stoi(line.substr(9));
        }
        else if (line.find("NumBuildings=") == 0) {
            numBuildings = stoi(line.substr(13));
        }
        else if (line.find("NumInitialCitizens=") == 0) {
            numInitialCitizens = stoi(line.substr(19));
        }
        else if (line.find("RefreshRate=") == 0) {
            refreshRate = stoi(line.substr(12));
        }
        else if (line.find("TimeLimit=") == 0) {
            timeLimit = stoi(line.substr(10));
        }
    }
    configFile.close();

    if (citySize == 0) {
        cerr << "Warning: City size not specified in the configuration file. Using default size: " << defaultCitySize << endl;
        citySize = defaultCitySize;
    }

    // Check and adjust citySize if needed
    const int MINIMUM_CITY_SIZE = 5; // Define your minimum city size
    if (citySize < MINIMUM_CITY_SIZE) {
        cerr << "Warning: City size is below the minimum value. Adjusting to the minimum size." << endl;
        citySize = MINIMUM_CITY_SIZE;
    }

    City city(citySize);

    // Getting user input
    int xCoordinate, yCoordinate;
    cout << "Enter the x and y coordinates to spawn a citizen (separated by space): ";
    cin >> xCoordinate >> yCoordinate;

    // Spawning a citizen at the specified coordinates
    city.spawnCitizen(xCoordinate, yCoordinate);

  

    // Accessing grid
    char** grid = city.getGrid();
    // Accessing populations
    int** populations = city.getPopulations();
    // Accessing citySize
    int size = city.getCitySize();

     ifstream regionFile("region.csv");
    if (!regionFile.is_open()) {
        cerr << "Error: Could not open region file." << endl;
        return 1;
    }

    int lineNumber = 0;
    while (getline(regionFile, line)) {
        if (lineNumber >= citySize) {
            cerr << "Error: Too many lines in the region file." << endl;
            return 1;
        }

        istringstream rowStream(line);
        string cell;
        int x = 0;
        while (getline(rowStream, cell, ',')) {
            if (x >= citySize) {
                cerr << "Error: Too many columns in the region file." << endl;
                return 1;
            }

            char cellValue = cell[0];
            if (cellValue == PowerLineRoad) {
                city.build(x, lineNumber);
            }
            else if (cellValue == ResidentialZone && numInitialCitizens > 0) {
                city.spawnCitizen(x, lineNumber);
                numInitialCitizens--;
            }
            x++;
        }

        if (x != citySize) {
            cerr << "Error: Region file dimensions do not match city size." << endl;
            return 1;
        }

        lineNumber++;
    }
    regionFile.close();

    if (lineNumber != citySize) {
        cerr << "Error: Not enough lines in the region file." << endl;
        return 1;
    }

    int currentTimeStep = 0;
    bool growthOccurred = true;

    while (growthOccurred && currentTimeStep < timeLimit) {
        residential.residentialGrowth(city.grid, city.populations, city.citySize);
        if (currentTimeStep % refreshRate == 0) {
            cout << "Time Step: " << currentTimeStep << endl;
            city.displayCity();
            cout << "Available Workers: " << city.getNumCitizens() << endl;
            cout << "Available Goods: " << numBuildings << endl;
        }

        bool changesOccurred = false;

        for (int y = 0; y < citySize; y++) {
            for (int x = 0; x < citySize; x++) {
                if (city.canGrow(x, y)) {
                    int totalAdjacentPopulations = city.calculateTotalAdjacentPopulations(x, y);

                    if (city.shouldGrowCommercial(x, y, totalAdjacentPopulations)) {
                        city.growCommercial(x, y);
                        changesOccurred = true;
                    }
                    else if (city.shouldGrowIndustrial(x, y, totalAdjacentPopulations)) {
                        city.growIndustrial(x, y);
                        changesOccurred = true;
                    }

                    // Add more growth rules here
                }
            }
        }

        if (!changesOccurred) {
            growthOccurred = false;
        }

        currentTimeStep++;
    }

    return 0;
}

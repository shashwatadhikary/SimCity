# SimCity
Kaya Miller, Evan Press, Shashwat Adhikary

In this SimCity project, we are using a region and config file and then initializing them into our main to create an output of a city with buildings, citizens, and different zones.

## Instructions
To compile the program add the region and config files into the same directory as the main file in an IDE of your choice and execute the program.

Make sure they are in the same dicretory or the program will not compile and run properly.

If using the GNU C++ compiler, run the command g++ City.cpp, Region.csv and Config.txt and proceed to run the said executable.

Feature Allocation Breakdown:

Evan Press: Industrial and Wiki creation

Kaya Miller: Residential and city.cpp

Shashwat Adhikary: Commercial creation






## Feature Summary 
  - Creates city and populates city with a grid.
  - Populates it with zones and citizens.
  - Adds the roads, powerlines, and buildings.
  - Grows the grid and calculates the population.
  - Grows the different commercial, residential, and industrial industries.
  - In the main, it creates the default city size, refresh rate, and the timelimit.
  - Initializes those variables to create growth and show time progress.
  - Produces error code if things are not initialized properly

## Examples
These are code snippets from our main showing the code's functionality.

## Initializing Values
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

## Creating and Populating the City
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

## Displaying the City
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

## Growth Functions
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

## Opening the Config File and Utilizing it
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

## Creating the City Size
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

## Utilizing Regin File
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

## Adding Growth and Returning the Data
        int currentTimeStep = 0;
    bool growthOccurred = true;

    while (growthOccurred && currentTimeStep < timeLimit) {
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
## Debugging Algorithms
        if (lineNumber != citySize) {
        cerr << "Error: Not enough lines in the region file." << endl;
        return 1;
    }
    if (x != citySize) {
            cerr << "Error: Region file dimensions do not match city size." << endl;
            return 1;
        }
        if (lineNumber >= citySize) {
            cerr << "Error: Too many lines in the region file." << endl;
            return 1;
        }

## Residential
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



## Industrial
    void Industrial::industrialGrowth(char** grid, int** populations, int citySize) {
    for (int y = 0; y < citySize; y++) {
        for (int x = 0; x < citySize; x++) {
            if (grid[y][x] == 'I') {  // Check if the cell is an IndustrialZone
                int adjacentPopulation = countAdjacentPopulation(populations, citySize, x, y);
                if (adjacentPopulation >= 2) {
                    populations[y][x]++;  // Increase the population if the conditions are met
                }
            }
        }
    }


    int Industrial::countAdjacentPopulation(int** populations, int citySize, int x, int y) {
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

## Creating class instance
    Residential residential; // Create an instance of the Residential class
    Industrial industrial; // Create an instance of the Indutrial class

## Growth Functions

    void Industrial::industrialGrowth(char** grid, int** populations, int citySize) {
    for (int y = 0; y < citySize; y++) {
        for (int x = 0; x < citySize; x++) {
            if (grid[y][x] == 'I') {  // Check if the cell is an IndustrialZone
                int adjacentPopulation = countAdjacentPopulation(populations, citySize, x, y);
                if (adjacentPopulation >= 2) {
                    populations[y][x]++;  // Increase the population if the conditions are met
                }
            }
        }
    }
    }

    int Industrial::countAdjacentPopulation(int** populations, int citySize, int x, int y) {
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

    bool Industrial::hasEnoughAvailableWorkers(int availableWorkers) {
    return availableWorkers >= 2;  // Check if there are at least 2 available workers
    }

    //These are the functions the industrial sector uses to evaluate and perform population/industrial growth.^

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

    //These are the functions used to determine residential/population growth.^




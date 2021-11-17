#include <string>
#include <iostream>
#include <fstream>
#include <iostream>

#include "floor.h"
#include "utils/color.h"


using namespace std; 

// Read the map from a filename and create the environment (2D vector of Cells)
void Floor::readMap(string filename) {
    string fileStr;
    std::ifstream infile(filename);

    int rowNum = 0;
    for (string line; getline(infile, line); ) {

        vector<Cell> row;
        for (int i = 0; i < line.length(); i++) {
            row.emplace_back(line[i]);
        }
        environment.push_back(row);
        rowNum += 1;
    }

    // Read the 

}


// Loop through the environment and return the char version of it
vector<vector<char>> Floor::getEnvironmentChar() {
    vector<vector<char>> charEnvironment;
    for (int i = 0; i < environment.size(); i++) {
        vector<char> charRow;
        for (int j = 0; j < environment[0].size(); j++) {
            charRow.push_back(environment[i][j].getChar());
        }
        charEnvironment.push_back(charRow);
    }
    return charEnvironment;
}

int Floor::chamberAt(int x, int y) {
    return 0;
}

int Floor::getHeight() {
    return HEIGHT;
}

int Floor::getWidth() {
    return WIDTH;
}

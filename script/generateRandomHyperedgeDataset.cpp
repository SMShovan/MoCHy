#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    return min + std::rand() % ((max + 1) - min);
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <output_file> <vector_length> <set_size_min> <set_size_max> <value_min> <value_max>\n";
        return 1;
    }

    std::string outputFileName = argv[1];
    int vectorLength = std::stoi(argv[2]);
    int setSizeMin = std::stoi(argv[3]);
    int setSizeMax = std::stoi(argv[4]);
    int valueMin = std::stoi(argv[5]);
    int valueMax = std::stoi(argv[6]);

    // Seed the random number generator
    std::srand(std::time(0));

    // Create the vector of sets
    std::vector<std::set<int>> vec(vectorLength);
    for (int i = 0; i < vectorLength; ++i) {
        int setSize = randomInt(setSizeMin, setSizeMax);
        while (vec[i].size() < setSize) {
            vec[i].insert(randomInt(valueMin, valueMax));
        }
    }

    // Write the vector of sets to the file
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << outputFileName << "\n";
        return 1;
    }

    for (const auto& s : vec) {
        for (const auto& elem : s) {
            outputFile << elem << " ";
        }
        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Data successfully written to " << outputFileName << "\n";

    return 0;
}

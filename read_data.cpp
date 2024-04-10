#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;


void read_data(string path, vector< vector<int> >& node2hyperedge, vector< vector<int> >& hyperedge2node, vector< unordered_set<int> >& hyperedge2node_set)
{
    ifstream graphFile(path.c_str()); // Open the file at the given path for reading.
    string line; // Variable to hold each line read from the file.
    int num_hyperedges = 0; // Counter to keep track of the number of hyperedges processed.

    // Read the file line by line. Each line represents a hyperedge.
    while (getline(graphFile, line))
    {
        unordered_set<int> tokens_set; // To hold unique nodes for the current hyperedge.
        vector<int> tokens; // To hold all nodes for the current hyperedge in order.
        bool EOL = false; // End Of Line flag to indicate when the line has been fully processed.

        // Process each node in the line.
        while (!EOL)
        {
            size_t pos = line.find(","); // Find the next comma in the line.
            int idx; // To hold the node index after conversion from string to int.

            // Check if there are no more commas, indicating the end of the line.
            if(pos == string::npos){
                pos = line.size(); // Set pos to the end of the line.
                EOL = true; // Set the flag to true since this is the last node in the line.
                idx = stoi(line); // Convert the last portion of the line to an integer node index.
            }else{
                // If there is a comma, extract the node index up to the comma.
                idx = stoi(line.substr(0, pos));
                // Remove the processed part of the line along with the comma.
                line.erase(0, pos + 1);
            }

            // Ensure there is a vector for the current node in node2hyperedge.
            while(idx >= node2hyperedge.size()){
                node2hyperedge.push_back(vector<int>());
            }

            // If the current node does not already have the current hyperedge in its list, add it.
            if(node2hyperedge[idx].empty() || node2hyperedge[idx].back() != num_hyperedges){
                node2hyperedge[idx].push_back(num_hyperedges);
                tokens.push_back(idx); // Add the node to the current hyperedge's node list.
                tokens_set.insert(idx); // Insert the node into the set for uniqueness.
            }
        }

        // After processing all nodes in the line, add the collected data to the structures.
        hyperedge2node.push_back(tokens); // Add the list of nodes to hyperedge2node.
        hyperedge2node_set.push_back(tokens_set); // Add the set of unique nodes to hyperedge2node_set.

        num_hyperedges++; // Increment the hyperedge counter after processing the line.
    }

    graphFile.close(); // Close the file after reading all lines.
}
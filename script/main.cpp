#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include <climits>
#include <algorithm>

// Function to read the file and store contents in a vector of vectors
std::vector<std::vector<long long>> readFileToVector(const std::string& inputFileName) {
    std::vector<std::vector<long long>> vec;
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << inputFileName << "\n";
        return vec;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        long long num;
        std::vector<long long> temp;
        while (iss >> num) {
            temp.push_back(num);
        }
        vec.push_back(temp);
    }

    inputFile.close();
    return vec;
}

// Function to flatten a 2-D vector into a contiguous memory block with metadata
long long* flatten2DVector(const std::vector<std::vector<long long>>& vec, size_t& totalSize, std::vector<uintptr_t>& addressArray) {
    // Set default total size to 1MB
    size_t defaultSize = 1 * 1024 * 1024 / sizeof(long long);  // 1MB in terms of long long elements

    // Calculate the actual size needed
    size_t requiredSize = 0;
    for (const auto& v : vec) {
        requiredSize += v.size() + 4;  // Adding 4 for start, end, previous, and next addresses
    }

    // Ensure the total size is a multiple of 32 long long integers (256 bytes)
    size_t paddedSize = ((requiredSize + 31) / 32) * 32;

    // Use the larger of the default size or the calculated padded size
    totalSize = std::max(defaultSize, paddedSize);

    // Allocate contiguous memory
    long long* contiguousMemory = new(std::nothrow) long long[totalSize]();
    if (!contiguousMemory) {
        std::cerr << "Memory allocation failed\n";
        exit(1);
    }

    // Flatten the 2-D vector into the allocated memory with metadata
    long long* currentPtr = contiguousMemory;
    for (const auto& v : vec) {
        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&v.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&v.back());
        *reinterpret_cast<uintptr_t*>(currentPtr++) = startAddress;  // Start address
        *reinterpret_cast<uintptr_t*>(currentPtr++) = endAddress;    // End address
        *reinterpret_cast<uintptr_t*>(currentPtr++) = INT_MAX;       // Previous address (initially INT_MAX)
        *reinterpret_cast<uintptr_t*>(currentPtr++) = INT_MAX;       // Next address (initially INT_MAX)
        addressArray.push_back(startAddress);
        for (const auto& elem : v) {
            *currentPtr++ = elem;
        }
        // Pad the remaining space to ensure the length is a multiple of 32 long long integers
        while ((currentPtr - contiguousMemory) % 32 != 0) {
            *currentPtr++ = 0;
        }
    }

    return contiguousMemory;
}

// Function to print the contents of the contiguous memory block
void printFlattenedArray(const long long* contiguousMemory, size_t totalSize) {
    std::cout << "\nFlattened 1-D array contents:\n";
    for (size_t i = 0; i < totalSize; ++i) {
        std::cout << contiguousMemory[i] << " ";
    }
    std::cout << "\n";
}

// Red-Black Tree Node
struct RBTreeNode {
    uintptr_t key;
    RBTreeNode *left, *right, *parent;
    bool color; // true for Red, false for Black

    RBTreeNode(uintptr_t key) : key(key), left(nullptr), right(nullptr), parent(nullptr), color(true) {}
};

// Class representing Red-Black Tree
class RBTree {
private:
    RBTreeNode* root;

    void rotateLeft(RBTreeNode*& root, RBTreeNode*& pt) {
        RBTreeNode* pt_right = pt->right;

        pt->right = pt_right->left;

        if (pt->right != nullptr)
            pt->right->parent = pt;

        pt_right->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;

        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(RBTreeNode*& root, RBTreeNode*& pt) {
        RBTreeNode* pt_left = pt->left;

        pt->left = pt_left->right;

        if (pt->left != nullptr)
            pt->left->parent = pt;

        pt_left->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;

        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(RBTreeNode*& root, RBTreeNode*& pt) {
        RBTreeNode* parent_pt = nullptr;
        RBTreeNode* grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != false) && (pt->parent->color == true)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                RBTreeNode* uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == true) {
                    grand_parent_pt->color = true;
                    parent_pt->color = false;
                    uncle_pt->color = false;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->right) {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }

                    rotateRight(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            } else {
                RBTreeNode* uncle_pt = grand_parent_pt->left;

                if ((uncle_pt != nullptr) && (uncle_pt->color == true)) {
                    grand_parent_pt->color = true;
                    parent_pt->color = false;
                    uncle_pt->color = false;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->left) {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }

                    rotateLeft(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }

        root->color = false;
    }

    void inorderHelper(RBTreeNode* root) {
        if (root == nullptr)
            return;

        inorderHelper(root->left);
        std::cout << root->key << " ";
        inorderHelper(root->right);
    }

    RBTreeNode* BSTInsert(RBTreeNode* root, RBTreeNode* pt) {
        if (root == nullptr)
            return pt;

        if (pt->key < root->key) {
            root->left = BSTInsert(root->left, pt);
            root->left->parent = root;
        } else if (pt->key > root->key) {
            root->right = BSTInsert(root->right, pt);
            root->right->parent = root;
        }

        return root;
    }

    void fixDeleteViolation(RBTreeNode*& root, RBTreeNode*& x) {
        while (x != root && x->color == false) {
            if (x == x->parent->left) {
                RBTreeNode* sibling = x->parent->right;
                if (sibling->color == true) {
                    sibling->color = false;
                    x->parent->color = true;
                    rotateLeft(root, x->parent);
                    sibling = x->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->color == false) &&
                    (sibling->right == nullptr || sibling->right->color == false)) {
                    sibling->color = true;
                    x = x->parent;
                } else {
                    if (sibling->right == nullptr || sibling->right->color == false) {
                        if (sibling->left != nullptr) sibling->left->color = false;
                        sibling->color = true;
                        rotateRight(root, sibling);
                        sibling = x->parent->right;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = false;
                    if (sibling->right != nullptr) sibling->right->color = false;
                    rotateLeft(root, x->parent);
                    x = root;
                }
            } else {
                RBTreeNode* sibling = x->parent->left;
                if (sibling->color == true) {
                    sibling->color = false;
                    x->parent->color = true;
                    rotateRight(root, x->parent);
                    sibling = x->parent->left;
                }
                if ((sibling->left == nullptr || sibling->left->color == false) &&
                    (sibling->right == nullptr || sibling->right->color == false)) {
                    sibling->color = true;
                    x = x->parent;
                } else {
                    if (sibling->left == nullptr || sibling->left->color == false) {
                        if (sibling->right != nullptr) sibling->right->color = false;
                        sibling->color = true;
                        rotateLeft(root, sibling);
                        sibling = x->parent->left;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = false;
                    if (sibling->left != nullptr) sibling->left->color = false;
                    rotateRight(root, x->parent);
                    x = root;
                }
            }
        }
        x->color = false;
    }

public:
    RBTree() : root(nullptr) {}

    void insert(uintptr_t key) {
        RBTreeNode* pt = new RBTreeNode(key);
        root = BSTInsert(root, pt);
        fixViolation(root, pt);
    }

    void inorder() {
        inorderHelper(root);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFileName = argv[1];

    // Read the input file and store contents in a vector of vectors
    std::vector<std::vector<long long>> vec = readFileToVector(inputFileName);

    // Print the 2-D vector contents
    std::cout << "2-D Vector contents:\n";
    for (const auto& v : vec) {
        for (const auto& num : v) {
            std::cout << num << " ";
        }
        std::cout << "\n";
    }

    // Flatten the 2-D vector into a contiguous memory block with metadata
    size_t totalSize;
    std::vector<uintptr_t> addressArray;
    long long* contiguousMemory = flatten2DVector(vec, totalSize, addressArray);

    // Print the flattened array contents
    printFlattenedArray(contiguousMemory, totalSize);

    // Create and populate the Red-Black Tree with the starting addresses
    RBTree rbtree;
    for (const auto& address : addressArray) {
        rbtree.insert(address);
    }

    // Print the inorder traversal of the Red-Black Tree
    std::cout << "\nInorder Traversal of Created Red-Black Tree:\n";
    rbtree.inorder();
    std::cout << "\n";

    // Free the allocated memory
    delete[] contiguousMemory;

    return 0;
}

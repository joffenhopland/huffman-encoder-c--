#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// Huffman Tree node
struct Node {
    char charactor;  // The character represented by this node
    int frequency;   // frequency of the character
    Node* left;      // left child
    Node* right;     // right child

    // initialize Node
    Node(char charactor, int frequency, Node* left = nullptr, Node* right = nullptr)
        : charactor(charactor), frequency(frequency), left(left), right(right) {}
};

        // class to compare two nodes based on frequency
struct CompareNode {
    bool operator()(Node* left, Node* right) {
        return left->frequency > right->frequency;
    }
};

    // Generate Huffman codes recursively for a given Huffman tree root
void generate_huffman_codes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    // Base case: if the root is null, just return
    if (root == nullptr) return;

    // If the node is a leaf node, add its Huffman code to the map
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->charactor] = code;
    }

    // do it again for the left and right children
    generate_huffman_codes(root->left, code + "0", huffmanCodes);
    generate_huffman_codes(root->right, code + "1", huffmanCodes);
}

// encoding
tuple<string, unordered_map<char, string>, Node*> huffman_encoding(const string& s) {
    unordered_map<char, int> frequency;

    // Count frequency of each character in the string
    for (char c : s) {
        frequency[c]++;
    }

    // priority queue to build the Huffman tree
    priority_queue<Node*, vector<Node*>, CompareNode> priorityQueue;

    // Initialize the priority queue with leaf nodes for each character and frequency
    for (const auto& entry : frequency) {
        priorityQueue.push(new Node(entry.first, entry.second));
    }

    // Build the tree
    while (priorityQueue.size() > 1) {
        Node* left = priorityQueue.top(); priorityQueue.pop();
        Node* right = priorityQueue.top(); priorityQueue.pop();

        // Mrge two nodes with the smallest frequencies
        Node* merged = new Node('\0', left->frequency + right->frequency, left, right);
        priorityQueue.push(merged);
    }

    // remaining node is the root
    Node* root = priorityQueue.top();

    // Generate Huffman codes for each character
    unordered_map<char, string> huffmanCodes;
    generate_huffman_codes(root, "", huffmanCodes);

    // Generate compressed data string
    string compressedData = "";
    for (char c : s) {
        compressedData += huffmanCodes[c];
    }

    return make_tuple(compressedData, huffmanCodes, root);
}

// decode
string huffman_decoding(const string& compressedData, Node* root) {
    string decodedData = "";
    Node* current = root;

    // Traverse the tree
    for (char bit : compressedData) {
        current = (bit == '0') ? current->left : current->right;

        // append the character to the decoded dataif we reach a leaf node
        if (current->left == nullptr && current->right == nullptr) {
            decodedData += current->charactor;
            current = root;
        }
    }

    return decodedData;
}

int main() {

    string s = "Huffman";

    auto result = huffman_encoding(s);

    cout << "Huffman Codes: ";
    for (const auto& entry : get<1>(result)) {
        cout << entry.first << ": " << entry.second << ", ";
    }
    cout << endl;

    cout << "Compressed Data: " << get<0>(result) << endl;
    cout << "Original Size: " << s.size() * 8 << " bits" << endl;
    cout << "Compressed Size: " << get<0>(result).size() << " bits" << endl;

    string decodedData = huffman_decoding(get<0>(result), get<2>(result));
    cout << "Decoded Data: " << decodedData << endl;

    return 0;
}

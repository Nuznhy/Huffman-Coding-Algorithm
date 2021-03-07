#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>

class Node {
public:
    int number;
    char symbol;
    Node* left, * right;

    Node() { left = right = NULL; }

    Node(Node* L, Node* R) {
        left = L;
        right = R;
        number = L->number + R->number;
    }
};

struct customCompare {
    bool operator() (const Node* L, const Node* R) const {
        return (L->number < R->number);
    }
};

std::vector<bool> code;
std::map< char, std::vector<bool> > table;

void BuildTable(Node* root) {
    if (root->left != NULL) {
        code.push_back(0);
        BuildTable(root->left);
    }
    if (root->right != NULL) {
        code.push_back(1);
        BuildTable(root->right);
    }
    if (root->left == NULL && root->right == NULL) {
        table[root->symbol] = code;
    }
    if (code.empty()) {
        return;
    }
    code.pop_back();
}

int main() {
    ///  Count Symbols   ///
    std::ifstream file("sample.txt", std::ios::out | std::ios::binary);
    std::map<char, int> symbol_map;
    while (!file.eof()) {
        char c = file.get();
        symbol_map[c]++;
    }

    /// Making start Nodes List ///
    std::list<Node*> node_list;

    for (std::map<char, int>::iterator itr = symbol_map.begin(); itr != symbol_map.end(); ++itr) {
        Node* pointer = new Node;
        pointer->symbol = itr->first;
        pointer->number = itr->second;
        node_list.push_back(pointer);
    }

    /// Creating Tree ///
    while (node_list.size() != 1) {
        node_list.sort(customCompare());

        Node* L = node_list.front();
        node_list.pop_front();
        Node* R = node_list.front();
        node_list.pop_front();

        Node* parent = new Node(L, R);
        node_list.push_back(parent);
    }
    Node* root = node_list.back();
    /// Creating "Code-Symbol" ///
    BuildTable(root);

    file.clear();
    file.seekg(0);

    std::ofstream g("output.bin", std::ios::out | std::ios::binary);

    int count = 0; char buf = 0;
    while (!file.eof())
    {
        char c = file.get();
        std::vector<bool> x = table[c];
        for (int n = 0; n < x.size(); n++)
        {
            buf = buf | x[n] << (7 - count);
            count++;
            if (count == 8) { 
                count = 0; 
                g << buf; 
                buf = 0; 
            }
        }
    }

    file.close();
    g.close();

    std::ifstream F("output.bin", std::ios::in | std::ios::binary);

    Node* p = root;
    count = 0; char byte;
    byte = F.get();
    while (!F.eof())
    {
        bool b = byte & (1 << (7 - count));
        if (b) p = p->right; else p = p->left;
        if (p->left == NULL && p->right == NULL) { 
            std::cout << p->symbol; 
            p = root; 
        }
        count++;
        if (count == 8) { 
            count = 0; 
            byte = F.get(); 
        }
    }

    F.close();

    return 0;

    F.close();
}

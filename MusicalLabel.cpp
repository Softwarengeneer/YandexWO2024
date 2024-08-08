#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Permissions {
    int read = 0;  // 1: granted, -1: blocked
    int write = 0; // 1: granted, -1: blocked
};

struct Node {
    Permissions permissions;
    map<string, Node> children;
};

unordered_map<string, Node> users;

vector<string> splitPath(const string &path) {
    vector<string> components;
    stringstream ss(path);
    string token;
    while (getline(ss, token, '/')) {
        if (!token.empty()) {
            components.push_back(token);
        }
    }
    return components;
}

void updatePermissions(Node &node, const vector<string> &pathComponents, int index, const string &mode, int value) {
    if (index == pathComponents.size()) {
        if (mode == "read") {
            node.permissions.read = value;
        } else {
            node.permissions.write = value;
        }
        return;
    }
    string current = pathComponents[index];
    updatePermissions(node.children[current], pathComponents, index + 1, mode, value);
}

bool checkPermission(const Node &node, const vector<string> &pathComponents, int index, const string &mode, int &effectiveRead, int &effectiveWrite) {
    if (node.permissions.read != 0) effectiveRead = node.permissions.read;
    if (node.permissions.write != 0) effectiveWrite = node.permissions.write;
    
    if (index == pathComponents.size()) {
        if (mode == "read") return effectiveRead == 1;
        return effectiveWrite == 1;
    }
    
    string current = pathComponents[index];
    auto it = node.children.find(current);
    if (it != node.children.end()) {
        return checkPermission(it->second, pathComponents, index + 1, mode, effectiveRead, effectiveWrite);
    } else {
        if (mode == "read") return effectiveRead == 1;
        return effectiveWrite == 1;
    }
}

int main() {
    string command, user, mode, path;
    while (cin >> command >> user >> mode >> path) {
        vector<string> pathComponents = splitPath(path);
        if (command == "grant") {
            updatePermissions(users[user], pathComponents, 0, mode, 1);
            cout << "done" << endl;
        } else if (command == "block") {
            updatePermissions(users[user], pathComponents, 0, mode, -1);
            cout << "done" << endl;
        } else if (command == "check") {
            int effectiveRead = 0, effectiveWrite = 0;
            if (checkPermission(users[user], pathComponents, 0, mode, effectiveRead, effectiveWrite)) {
                cout << "allowed" << endl;
            } else {
                cout << "forbidden" << endl;
            }
        }
    }

    return 0;
}


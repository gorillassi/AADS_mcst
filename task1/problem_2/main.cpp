#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Node {
    string value;
    Node* next;
    Node(string val) : value(val), next(nullptr) {}
};

class Stack {
private:
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}

    ~Stack() {
        while (!isEmpty()) pop();
    }

    void push(const string& val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    string top() const {
        return topNode ? topNode->value : "";
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    void clear() {
        while (!isEmpty()) pop();
    }
};

int main() {
    Stack backStack, forwardStack;
    string current = "homepage";

    string line;
    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "visit") {
            string url; iss >> url;
            backStack.push(current);
            current = url;
            forwardStack.clear();
            cout << current << endl;

        } else if (command == "back") {
            int steps; iss >> steps;
            while (steps-- > 0 && !backStack.isEmpty()) {
                forwardStack.push(current);
                current = backStack.top();
                backStack.pop();
            }
            cout << current << endl;

        } else if (command == "forward") {
            int steps; iss >> steps;
            while (steps-- > 0 && !forwardStack.isEmpty()) {
                backStack.push(current);
                current = forwardStack.top();
                forwardStack.pop();
            }
            cout << current << endl;
        }
    }

    return 0;
}
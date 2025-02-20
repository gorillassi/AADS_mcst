#include <iostream>
#include <cstring>

#define PATH_MAX 4096

struct StackNode {
    char dir[PATH_MAX];
    StackNode* next;
};

struct Stack {
    StackNode* top;

    Stack() : top(nullptr) {}

    void push(const char* directory) {
        StackNode* node = new StackNode();
        strcpy(node->dir, directory);
        node->next = top;
        top = node;
    }

    void pop() {
        if (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void reversePrint() const {
        reversePrintHelper(top);
    }

private:
    void reversePrintHelper(StackNode* node) const {
        if (!node) return;
        reversePrintHelper(node->next);
        std::cout << "/" << node->dir;
    }
};

int main() {
    char path[PATH_MAX];
    std::cin.getline(path, PATH_MAX);

    Stack stack;
    char* token = strtok(path, "/");

    while (token != nullptr) {
        if (strcmp(token, ".") == 0) {
            // Ignore '.'
        } else if (strcmp(token, "..") == 0) {
            if (stack.isEmpty()) {
                std::cerr << "Going upper than the root directory is not possible." << std::endl;
                return 1;
            }
            stack.pop();
        } else if (strlen(token) > 0) {
            stack.push(token);
        }
        token = strtok(nullptr, "/");
    }

    if (stack.isEmpty()) {
        std::cout << "/" << std::endl;
    } else {
        stack.reversePrint();
        std::cout << std::endl;
    }

    return 0;
}
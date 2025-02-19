#include <iostream>
#include <cstring>

const int MAX_PATH = 4096;

struct Node {
    char dir[256];
    Node* next;
};

struct Stack {
    Node* top;

    Stack() : top(nullptr) {}

    void push(const char* directory) {
        Node* node = new Node;
        strncpy(node->dir, directory, 255);
        node->dir[255] = '\0';
        node->next = top;
        top = node;
    }

    void pop() {
        if (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void clear() {
        while (!isEmpty()) pop();
    }

    ~Stack() { clear(); }
};

void splitPath(const char* path, char components[][256], int& count) {
    count = 0;
    const char* start = path + 1; // skip leading '/'
    const char* end;
    while (*start) {
        while (*start == '/') start++;
        if (!*start) break;
        end = strchr(start, '/');
        int len = end ? end - start : strlen(start);
        strncpy(components[count], start, len);
        components[count][len] = '\0';
        count++;
        if (!end) break;
        start = end;
    }
}

std::string canonicalPath(const char* path) {
    if (strlen(path) > MAX_PATH) return "Path exceeds maximum length.";

    Stack stack;
    char components[512][256];
    int count;
    splitPath(path, components, count);

    for (int i = 0; i < count; i++) {
        if (strcmp(components[i], "..") == 0) {
            if (stack.isEmpty()) return "Going upper than the root directory is not possible.";
            stack.pop();
        } else if (strcmp(components[i], ".") != 0) {
            stack.push(components[i]);
        }
    }

    if (stack.isEmpty()) return "/";

    std::string result;
    Stack reverseStack;

    while (!stack.isEmpty()) {
        reverseStack.push(stack.top->dir);
        stack.pop();
    }

    while (!reverseStack.isEmpty()) {
        result += "/";
        result += reverseStack.top->dir;
        reverseStack.pop();
    }

    return result;
}

void test(const char* input, const char* expected) {
    std::string result = canonicalPath(input);
    std::cout << "Input: " << input << "\nExpected: " << expected << "\nGot: " << result << "\n";
    std::cout << ((result == expected) ? "PASS\n" : "FAIL\n") << "----------------------\n";
}

int main() {
    test("/", "/");
    test("/home/", "/home");
    test("/home////////////////////Downloads/////", "/home/Downloads");
    test("/home/./Downloads/Films/../Stories/", "/home/Downloads/Stories");
    test("/../", "Going upper than the root directory is not possible.");
    test("/a/b/c/../../d/", "/a/d");
    return 0;
}

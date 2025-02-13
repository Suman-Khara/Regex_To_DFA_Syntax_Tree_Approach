#include<bits/stdc++.h>
using namespace std;

bool isOperator(char c) {
    return c == '|' || c == '.' || c == '*';
}

string infixToPostfix(string regex) {
    string modified;
    stack<char> ops;
    unordered_map<char, int> precedence = { {'*', 3}, {'.', 2}, {'|', 1} };
    for (int i = 0; i < regex.length(); i++) {
        char c1 = regex[i];
        modified += c1;
        if (i + 1 < regex.length()) {
            char c2 = regex[i + 1];
            if ((!isOperator(c1) && c1 != '(' && c1 != ')') || c1 == '*' || c1 == ')') {
                if ((!isOperator(c2) && c2 != ')' && c2 != '*') || c2 == '(')
                    modified += '.';
            }
        }
    }
    //cout << "Modified Regex: " << modified << endl;
    string postfix;
    for (char c : modified) {
        if (!isOperator(c) && c != '(' && c != ')')
            postfix += c;
        else if (c == '(')
            ops.push(c);
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                postfix += ops.top();
                ops.pop();
            }
            ops.pop();
        }
        else {
            while (!ops.empty() && precedence[ops.top()] >= precedence[c]) {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        postfix += ops.top();
        ops.pop();
    }
    //cout << "Postfix: " << postfix << endl;
    return postfix;
}

class TreeNode {
public:
    char value;
    int position;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char value, int position) {
        this->value = value;
        this->position = position;
        this->left = NULL;
        this->right = NULL;
    }
};

class SyntaxTree {
public:
    unordered_map<int, set<int>> followpos;
    TreeNode* root;
    int position = 0;
    set<char> alphabet;
    unordered_map<int, char> positions;
    unordered_map<int, vector<int>> tree;
    SyntaxTree(string regex) {
        string postfix = infixToPostfix("(" + regex + ")" + ".#");
        stack<TreeNode*> s;
        for (char c : postfix) {
            if (!isOperator(c)) {
                s.push(new TreeNode(c, position));
                positions[position] = c;
                position++;
            }
            else {
                TreeNode* node = new TreeNode(c, position++);
                node->right = s.top();
                s.pop();
                if (c == '*') {
                    node->left = node->right;
                    node->right = NULL;
                }
                else {
                    node->left = s.top();
                    s.pop();
                }
                s.push(node);
            }
            if (!isOperator(c) && c != '#')
                alphabet.insert(c);
        }
        root = s.top();
        computeFollowpos(root);
        //makeTree(root);
        //print();
    }
    bool nullabe(TreeNode* node) {
        if (node == NULL || !isOperator(node->value))
            return false;
        if (node->value == '*')
            return true;
        if (node->value == '.')
            return nullabe(node->left) && nullabe(node->right);
        if (node->value == '|')
            return nullabe(node->left) || nullabe(node->right);
        return false;
    }
    set<int> firstpos(TreeNode* node) {
        if (node == NULL)
            return {};
        if (!isOperator(node->value))
            return { node->position };
        if (node->value == '*')
            return firstpos(node->left);
        if (node->value == '.') {
            set<int> s = firstpos(node->left);
            if (nullabe(node->left)) {
                set<int> s1 = firstpos(node->right);
                s.insert(s1.begin(), s1.end());
            }
            return s;
        }
        if (node->value == '|') {
            set<int> s = firstpos(node->left);
            set<int> s1 = firstpos(node->right);
            s.insert(s1.begin(), s1.end());
            return s;
        }
        return {};
    }
    set<int> lastpos(TreeNode* node) {
        if (node == NULL)
            return {};
        if (!isOperator(node->value))
            return { node->position };
        if (node->value == '*')
            return lastpos(node->left);
        if (node->value == '.') {
            set<int> s = lastpos(node->right);
            if (nullabe(node->right)) {
                set<int> s1 = lastpos(node->left);
                s.insert(s1.begin(), s1.end());
            }
            return s;
        }
        if (node->value == '|') {
            set<int> s = lastpos(node->left);
            set<int> s1 = lastpos(node->right);
            s.insert(s1.begin(), s1.end());
            return s;
        }
        return {};
    }
    void computeFollowpos(TreeNode* node) {
        if (node == NULL)
            return;
        if (node->value == '.') {
            set<int> last = lastpos(node->left);
            set<int> first = firstpos(node->right);
            for (int i : last)
                followpos[i].insert(first.begin(), first.end());
        }
        else if (node->value == '*') {
            set<int> last = lastpos(node);
            set<int> first = firstpos(node);
            for (int i : last)
                followpos[i].insert(first.begin(), first.end());
        }
        computeFollowpos(node->left);
        computeFollowpos(node->right);
    }
    void makeTree(TreeNode* node) {
        if (node == NULL)
            return;
        if (node->left != NULL) {
            tree[node->position].push_back(node->left->position);
            makeTree(node->left);
        }
        if (node->right != NULL) {
            tree[node->position].push_back(node->right->position);
            makeTree(node->right);
        }
    }
    void print() {
        cout << "Syntax Tree Details:\n";
        cout << "Position\tValue\tLeft\tRight\n";
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            cout << node->position << "\t\t" << node->value << "\t";
            if (node->left != NULL) {
                cout << node->left->position << "\t";
                q.push(node->left);
            }
            else
                cout << "NULL\t";
            if (node->right != NULL) {
                cout << node->right->position << "\n";
                q.push(node->right);
            }
            else
                cout << "NULL\n";
        }
        cout << "Position\tFollowpos\n";
        for (auto& [pos, follow] : followpos) {
            cout << pos << "\t\t{ ";
            for (int i : follow) cout << i << " ";
            cout << "}\n";
        }
    }
};

class DFA {
public:
    set<int> states;
    set<char> alphabet;
    map<pair<int, char>, int> transitions;
    int start;
    set<int> finals;

    DFA(string regex) {
        start = 0;
        SyntaxTree tree(regex);
        alphabet = tree.alphabet;
        set<int> starting_int = tree.firstpos(tree.root);
        set<char> starting = { 'X' };
        for (int i : starting_int)
            starting.insert('0' + i);
        queue<set<char>> q;
        map<set<char>, int> stateMap;
        q.push(starting);
        stateMap[starting] = 0;
        states.insert(0);
        int count = 1, initial = 0;
        while (!q.empty()) {
            set<char> curr = q.front();
            q.pop();
            for (char c : tree.alphabet) {
                set<char> next_int;
                for (char d : curr) {
                    int i = d - '0';
                    if (tree.positions[i] == c)
                        next_int.insert(tree.followpos[i].begin(), tree.followpos[i].end());
                }
                set<char> next = { 'X' };
                for (int i : next_int)
                    next.insert('0' + i);
                if (stateMap.count(next) == 0) {
                    stateMap[next] = count;
                    q.push(next);
                    states.insert(count);
                    count++;
                }
                transitions[{stateMap[curr], c}] = stateMap[next];
            }
        }
        for (auto& [state, index] : stateMap) {
            for (char c : state) {
                int i = c - '0';
                if (tree.positions[i] == '#')
                    finals.insert(index);
            }
        }
        print();
    }
    void print() {
        cout << "DFA Details :\n";
        cout << "States: { ";
        for (int state : states) cout << state << " ";
        cout << "}\n";
        cout << "Alphabets: { ";
        for (char c : alphabet) cout << c << " ";
        cout << "}\n";
        cout << "Initial State: " << start << "\n";
        cout << "Final States: { ";
        for (int state : finals) cout << state << " ";
        cout << "}\n";
        cout << "Transition Table:\n";
        cout << "  Current   Input   Next     \n";

        for (auto& [key, nextState] : transitions) {
            int currState = key.first;
            char input = key.second;
            cout << "     " << setw(3) << currState << "      "
                << setw(3) << input << "     "
                << setw(3) << nextState << "     \n";
        }
    }
    bool isAccepted(string s) {
        if (s == "e") {
            if (finals.count(0)) {
                cout << "Result: ACCEPTED\n";
                return true;
            }
            else {
                cout << "Result: REJECTED\n";
                return false;
            }
        }
        int curr = start;
        cout << "  Current State   Input   Next  \n";
        for (char c : s) {
            cout << "        " << setw(3) << curr << "       "
                << setw(3) << c << "    ";
            curr = transitions[{curr, c}];
            cout << setw(3) << curr << "      \n";
        }
        if (finals.find(curr) != finals.end()) {
            cout << "Result: ACCEPTED\n";
            return true;
        }
        else {
            cout << "Result: REJECTED\n";
            return false;
        }
    }
};

int main() {
    string regex;
    cout << "Enter Regular Expression: ";
    cin >> regex;
    DFA dfa(regex);
    string input;
    cout << "Enter Input String: ";
    cin >> input;
    dfa.isAccepted(input);
}
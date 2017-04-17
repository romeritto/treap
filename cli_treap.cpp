#include <iostream>
#include <string>
#include <map>

#include "treap.hpp"

using namespace std;

enum Commands {
    Insert,
    Contains,
    Erase,
    Ord,
    At,
    Size,
};

map<string, int> command_map = {
    {"insert", Insert},
    {"contains", Contains},
    {"erase", Erase},
    {"ord", Ord},
    {"at", At},
    {"size", Size},
};

int resolve_command(string str_cmd) {
    auto it = command_map.find(str_cmd);

    if (it != command_map.end()) {
        return (*it).second;
    } else {
        return -1;
    }
}

int main() {
    Treap<int> treap;
    string str_cmd;
    int x;

    while (cin >> str_cmd) {

        switch(resolve_command(str_cmd)) {
            case Insert   : cin >> x; cout << treap.insert(x) << endl; break;
            case Contains : cin >> x; cout << treap.contains(x) << endl; break;
            case Erase    : cin >> x; cout << treap.erase(x) << endl; break;
            case Ord      : cin >> x; cout << treap.order_of_key(x) << endl; break;
            case At       : cin >> x;
                            if (x >= 0 && x < treap.size())
                              cout << treap.find_by_order(x) << endl;
                            else
                              cout << -1 << endl;

                            break;
            case Size     : cout << treap.size() << endl; break;
            default       : cout << "Unknown command" << endl; break;
        }

    }

    return 0;
}
# Treap
My goal was to implement *treap* data structure with its basic operations in C++11. You can find it in the source files `treap.hpp` and `treap.tpp`.
## 1. API
`Treap<TKey>` is the main class. It stores elements of type `TKey` in the treap data structure. The keys in the structure are **unique**, hence it represents a *set* of `TKey` elements.

### 1.1. How to include and define your own treap
You have to include the header file `treap.hpp` which contains the structure `Treap<TKey>`. This structure only has a default constructor.
```c++
#include "treap.hpp"

int main() {
    Treap<pair<int, int>> pii_set;
    Treap<int> my_numbers;
}
```

### 1.2. Methods
* `bool insert(TKey x)`
    + Method inserts element `x` into treap
    + Returns `true` if `x` was inserted, `false` otherwise
* `bool erase(TKey x)`
    + Erases `x` from treap
    + Returns `true` if `x` was deleted, `false` otherwise 
* `bool contains(TKey x)`
    + Method returns `true` if the treap contains `x`, `false` otherwise
* `int order_of_key(TKey x)`
    + Method returns `0` indexed, in-order position of `x` in the treap
    + If `x` is not present in the treap `-1` is returned
* `TKey find_by_order(int pos)`
    + Returns element at position `pos`, where `pos` should be `0` indexed
    + `pos` must be a **valid position**
* `int size()`
    + Returns current number of elements in the treap

### 1.3. Sample code
```c++
#include <iostream>
#include "treap.hpp"

using namespace std;

int main() {
    Treap<int> nums;
    for (int i = 0; i < 10; ++i)
        if (i % 2 == 0)
            nums.insert(i);

    if (nums.contains(7) == false)
        cout << "nums does not contain 7" << endl;

    cout << "4 is on position " << nums.order_of_key(4) << endl;
    cout << "Position 3 contains " << nums.find_by_order(3) << endl;
    nums.erase(8);
    cout << "The size of nums is " << nums.size() << endl;

    return 0;
}
```

Output
```
nums does not contain 7
4 is on position 2
Position 3 contains 6
The size of nums is 4
```

## 2. Basic CLI 
File `cli_treap.cpp` contains a simple interpreter of commands for treap of *intergers*. Commands have same names and parameters as methods of `Treap<TKey>` class with 2 exceptions:
1. `ord` = `oder_of_key` 
2. `at` = `find_by_order`

### 2.1. Example
Input
```
insert 5
at 1
insert 8
insert 8
contains 4
ord 5
size
erase 5
size
```

Output
```
1       // 5 successfully added
-1      // 1. position is blank (we are using 0 indexed positions)
1       // 8 added
0       // 8 not added (it is already in the treap)
0       // treap does not contain 4
0       // position of 5 is 0 
2       // treap contains two elements (5 and 8)
1       // 5 erased
1       // treap contains only 8
```

## 3. Contact me
If you find something unclear or if you have a suggestion, do not hesitate to write me an email at r.sobkuliak(at)gmail.com

## 4. Resources
My main resource was [a Wikipedia article about Treap](https://en.wikipedia.org/wiki/Treap) and [an implementation at Geeks for geeks](http://www.geeksforgeeks.org/treap-set-2-implementation-of-search-insert-and-delete/).

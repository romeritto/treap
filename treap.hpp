/*
 * treap.hpp
 * ---------
 *   Definitions for classes Treap and Node.
 *   Treap is implemented as set (every key is unique) and it uses max heap.
 *
 *   (c) 2017 Roman Sobkuliak [r.sobkuliak(at)gmail.com]
 *   This code is licensed under MIT license
 */

#ifndef TREAP_H
#define TREAP_H

#include <limits>
#include <random>

template <typename TKey>
class Treap {

    class Node {
    public:
        TKey key;
        unsigned priority;
        int tree_size = 1;
        Node *left = nullptr,
             *right = nullptr;

        static const unsigned MIN_PRIORITY = std::numeric_limits<unsigned>::min();
        static const unsigned MAX_PRIORITY = std::numeric_limits<unsigned>::max();

        int compute_tree_size();
        int left_tree_size() {
            return (left != nullptr) ? left->tree_size : 0;
        }

        Node(TKey init_key, unsigned init_priority) : key(init_key), priority(init_priority) {}
        Node(TKey init_key) : Node(init_key, _generate_priority()) {}
        ~Node();

    private:
        unsigned _generate_priority();
    };

public:
    bool insert(TKey key);
    bool contains(TKey key);
    bool erase(TKey key);
    int order_of_key(TKey key);
    TKey find_by_order(int key_position) {
        return _find_by_order(_treap_root, key_position + 1);
    }
    int size() {
        return (_treap_root != nullptr) ? _treap_root->tree_size : 0;
    }

    Treap() : _treap_root(nullptr) { };
    ~Treap() {
        delete _treap_root;
    };

private:
    typedef Node *Node_ptr;
    Node_ptr _treap_root;

    Node_ptr _rotate_right(Node_ptr x);
    Node_ptr _rotate_left(Node_ptr y);
    Node_ptr _insert(Node_ptr root, TKey key);
    Node_ptr _find(Node_ptr root, TKey key, int &key_position);
    Node_ptr _erase(Node_ptr root, TKey key, bool &erased);
    TKey _find_by_order(Node_ptr root, int key_position);
};

/*
 * Source code has to be included here, because we use templates.
 * For more information visit
 * http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
 */
#include "treap.tpp"

#endif
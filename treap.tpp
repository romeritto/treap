/*
 * treap.tpp
 * ---------
 *   Implements methods of classes Treap and Node defined in the header file treap.hpp
 *   We include treap.tpp into treap.hpp and not the other way around. Main reason for that
 *   is the use of templates.
 *
 *   (c) 2017 Roman Sobkuliak [r.sobkuliak(at)gmail.com]
 *   This code is licensed under MIT license
 */

template <typename TKey>
Treap<TKey>::Node::~Node() {
    delete left;
    delete right;
}

template <typename TKey>
unsigned Treap<TKey>::Node::_generate_priority() {
    static std::mt19937 engine{std::random_device{}()};
    static std::uniform_int_distribution<unsigned> distribution{MIN_PRIORITY, MAX_PRIORITY};
    return distribution(engine);
}

template <typename TKey>
inline int Treap<TKey>::Node::compute_tree_size() {
    int  left_size = (left != nullptr) ? left->tree_size : 0,
         right_size = (right != nullptr) ? right->tree_size : 0;

    return tree_size = left_size + 1 + right_size;
}

/*
 *           Following name-convention is used
 *
 *        x            Right rotate           y
 *       / \             ------>             / \
 *      y   T3           <------            T1  x
 *     / \             Left rotate             / \
 *    T1  T2                                  T2  T3
 *
 *    For further details see
 *            http://www.geeksforgeeks.org/treap-a-randomized-binary-search-tree/
 */

template <typename TKey>
typename Treap<TKey>::Node_ptr Treap<TKey>::_rotate_right(Node_ptr x) {
    Node_ptr  y = x->left,
              T2 = y->right;

    y->right = x;
    x->left = T2;

    x->compute_tree_size();
    y->compute_tree_size();

    return y;
}

template <typename TKey>
typename Treap<TKey>::Node_ptr Treap<TKey>::_rotate_left(Node_ptr y) {
    Node_ptr  x = y->right,
              T2 = x->left;

    x->left = y;
    y->right = T2;

    y->compute_tree_size();
    x->compute_tree_size();

    return x;
}

/* _find
 * -----
 *   Function returns pointer to node containing given key if such node exists in the tree.
 *   It also stores the in-order poisition of the key in the variable key_position (starting at 1).
 *   The value of key_position is undefined if key is not present in the tree.
 *   Variable passed as key_position must be set to 0 before calling _find.
 */
template <typename TKey>
typename Treap<TKey>::Node_ptr Treap<TKey>::_find(Node_ptr root, TKey key, int &key_position) {
    if (root == nullptr)
        return nullptr;

    Node_ptr answer = nullptr;
    int root_position = 1 + root->left_tree_size();

    if (key == root->key) {
        key_position += root_position;
        answer = root;
    } else if (key < root->key) {
        answer = _find(root->left, key, key_position);
    } else if (key > root->key) {
        key_position += root_position;
        answer = _find(root->right, key, key_position);
    }

    return answer;
}

template <typename TKey>
inline bool Treap<TKey>::contains(TKey key) {
    int key_position = 0;

    if (_find(_treap_root, key, key_position) != nullptr) {
        return true;
    } else {
        return false;
    }
}

template <typename TKey>
inline int Treap<TKey>::order_of_key(TKey key) {
    int key_position = 0;
    if (_find(_treap_root, key, key_position) != nullptr) {
        return key_position - 1;
    } else {
        return -1;
    }
}

/* _find_by_order
 * --------------
 *   Returns pointer to node at (one-indexed in-order) key_position in the tree.
 *   Method expects valid key_position.
 */
template <typename TKey>
TKey Treap<TKey>::_find_by_order(Node_ptr root, int key_position) {

    int root_position = 1 + root->left_tree_size();

    if (key_position == root_position) {
        return root->key;
    } else if (key_position < root_position) {
        return _find_by_order(root->left, key_position);
    } else {
        return _find_by_order(root->right, key_position - root_position);
    }
}

/* _insert
 * -------
 *   Inserts key into the tree. It returns root of the tree after insertion.
 *   Method does not check if key is present in the tree before insertion.
 */
template <typename TKey>
typename Treap<TKey>::Node_ptr Treap<TKey>::_insert(Node_ptr root, TKey key) {
    if (root == nullptr)
        return new Node(key);

    if (key < root->key) {
        root->left = _insert(root->left, key);

        if (root->left->priority > root->priority)
            root = _rotate_right(root);
    } else {
        root->right = _insert(root->right, key);

        if (root->right->priority > root->priority)
            root = _rotate_left(root);
    }

    root->compute_tree_size();

    return root;
}

template <typename TKey>
inline bool Treap<TKey>::insert(TKey key) {
    if (contains(key)) {
        return false;
    } else {
        _treap_root = _insert(_treap_root, key);
        return true;
    }
}

/* _erase
 * ------
 *   Deletes node with given key from the tree. Erased is set to true if key has been erased,
 *   otherwise the value does not change after call (erased should be set to false by caller).
 *   Method returns root node of the tree after deletion.
 */
template <typename TKey>
typename Treap<TKey>::Node_ptr Treap<TKey>::_erase(Node_ptr root, TKey key, bool &erased) {
    if (root == nullptr)
        return nullptr;

    Node_ptr left = root->left, right = root->right;

    if (key < root->key) {
        root->left = _erase(left, key, erased);
    }
    else if (key > root->key) {
        root->right = _erase(right, key, erased);
    }

    // key == root->key

    else if (left == nullptr && right == nullptr) {
        delete root;
        erased = true;
        root = nullptr;
    }
    else if (left == nullptr) {
        root->right = nullptr;
        delete root;
        erased = true;
        root = right;
    }
    else if (right == nullptr) {
        root->left = nullptr;
        delete root;
        erased = true;
        root = left;
    }

    // both children are not null

    else if (left->priority > right->priority) {
        root = _rotate_right(root);
        // root->right and not right defined earlier, because of rotation
        root->right = _erase(root->right, key, erased);
    } else {
        root = _rotate_left(root);
        // root->left and not left defined earlier, because of rotation
        root->left = _erase(root->left, key, erased);
    }

    if (root != nullptr)
        root->compute_tree_size();

    return root;
}

template <typename TKey>
inline bool Treap<TKey>::erase(TKey key) {
    bool erased = false;
    _treap_root = _erase(_treap_root, key, erased);

    return erased;
}

#include <fstream>
 
 
template<class T>
class Node {
private:
    Node *left, *right;
    T key;
    int height;
    
    template<class U> friend class AVL;
    
    static int Height(Node *N);
    explicit Node (T key_);
 
public:
    void calculate_height();
    static Node *create_node(T key);
};
 
 
template<class T>
class AVL {
    static Node<T> *root;
 
    static Node<T> *rotate_right(Node<T> *child2);
    static Node<T> *rotate_left(Node<T> *child1);
    static int getBalanceFactor(Node<T> *N);
 
    Node<T> *insert_node(Node<T> *node, T key);
    Node<T> *delete_node(Node<T> *current_root, T key);
    static Node<T> *leftest(Node<T> *node);
    T predecessor (T key, Node<T> *node);
    T successor (T key, Node<T> *node);
    void print(T x, T y, Node<T> *node);
 
public:
    void push (T key);
    void erase (T key);
    static T back();
    static T front();
    bool find (T key);
    T predecessor (T key);
    T successor (T key);
    void print(T x, T y);
};
 
 
template<class T>
int Node<T>::Height(Node<T> *N) {
    return (N ? N->height : 0);
}
 
template<class T>
Node<T>::Node (T key_) {
    key = key_;
    left = right = nullptr;
    height = 1;
}
 
template<class T>
void Node<T>::calculate_height() {
    this->height = std::max(Height(this->left), Height(this->right)) + 1;
}
 
template<class T>
Node<T> *Node<T>::create_node(T key) {
    auto *node = new Node(key);
    return node;
}
 
 
template<class T>
Node<T>* AVL<T>::root = nullptr;
 
 
template<class T>
Node<T>* AVL<T>::rotate_right(Node<T> *child2) {
    Node<T> *child1 = child2->left;
    Node<T> *aux = child1->right;
    child1->right = child2;
    child2->left = aux;
    child2->calculate_height();
    child1->calculate_height();
    return child1;
}
 
 
template<class T>
Node<T>* AVL<T>::rotate_left(Node<T> *child1) {
    Node<T> *child2 = child1->right;
    Node<T> *aux = child2->left;
    child2->left = child1;
    child1->right = aux;
    child1->calculate_height();
    child2->calculate_height();
    return child2;
}
 
 
template<class T>
int AVL<T>::getBalanceFactor(Node<T> *N) {
    return (N ? (N->Height(N->left) - N->Height(N->right)) : 0);
}
 
 
template<class T>
void AVL<T>::push(T key) {
    root = insert_node(root, key);
}
 
 
template<class T>
Node<T>* AVL<T>::insert_node(Node<T> *node, T key) {
    if (!node) return Node<T>::create_node(key);
 
    if (key < node->key) node->left = insert_node(node->left, key);
    else if (key > node->key) node->right = insert_node(node->right, key);
    else return node;
 
    node->calculate_height();
    int balanceFactor = getBalanceFactor(node);
 
    if (balanceFactor > 1) {
        if (key < node->left->key) return rotate_right(node);
        else if (key > node->left->key) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
    }
 
    if (balanceFactor < -1) {
        if (key > node->right->key) return rotate_left(node);
        else if (key < node->right->key) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
    }
 
    return node;
}
 
 
template<class T>
Node<T>* AVL<T>::leftest(Node<T> *node) {
    Node<T> *current = node;
    while (current->left) current = current->left;
    return current;
}
 
 
template<class T>
void AVL<T>::erase(T key) {
    root = delete_node(root, key);
}
 
 
template<class T>
Node<T>* AVL<T>::delete_node(Node<T> *current_root, T key) {
    if (!current_root) return current_root;
 
    if (key < current_root->key) current_root->left = delete_node(current_root->left, key);
    else if (key > current_root->key) current_root->right = delete_node(current_root->right, key);
    else {
        if (!current_root->left || !current_root->right) {
            Node<T> *aux = current_root->left ? current_root->left : current_root->right;
            if (!aux) aux = current_root, current_root = nullptr;
            else *current_root = *aux;
            delete aux;
        }
 
        else {
            Node<T> *aux = leftest(current_root->right);
            current_root->key = aux->key;
            current_root->right = delete_node(current_root->right, aux->key);
        }
    }
 
    if (!current_root) return current_root;
 
    current_root->calculate_height();
    int balanceFactor = getBalanceFactor(current_root);
 
    if (balanceFactor > 1) {
        if (getBalanceFactor(current_root->left) >= 0) return rotate_right(current_root);
        else {
            current_root->left = rotate_left(current_root->left);
            return rotate_right(current_root);
        }
    }
 
    if (balanceFactor < -1) {
        if (getBalanceFactor(current_root->right) <= 0) return rotate_left(current_root);
        else {
            current_root->right = rotate_right(current_root->right);
            return rotate_left(current_root);
        }
    }
 
    return current_root;
}
 
 
template<class T>
T AVL<T>::back() {
    return leftest(root)->key;
}
 
 
template<class T>
T AVL<T>::front() {
    return root->key;
}
 
template<class T>
bool AVL<T>::find(T key) {
    Node<T> *current_root = root;
    while (current_root && current_root->key != key) {
        if (key < current_root->key) current_root = current_root->left;
        else if (key > current_root->key) current_root = current_root->right;
        else return true;
    }
    return current_root && current_root->key == key;
}
 
template<class T>
T AVL<T>::predecessor(T key, Node<T> *node) {
    if (node->key == key)
        return node->key;
 
    Node <T> *son;
    if (node->left && node->right) {
        if (node->key >= key) son = node->left;
        else son = node->right;
    }
    else if (node->left) son = node->left;
    else if (node->right) son = node->right;
    else return node->key;
 
    int aux = predecessor(key, son);
    if (aux > key || node->key > key)
        return std::min(aux, node->key);
 
    return std::max(aux, node->key);
}
 
template<class T>
T AVL<T>::predecessor(T key) {
    return predecessor(key, root);
}
 
template<class T>
T AVL<T>::successor(T key, Node<T> *node) {
    if (node->key == key)
        return node->key;
 
    Node <T> *son;
    if (node->left && node->right) {
        if (node->key <= key) son = node->right;
        else son = node->left;
    }
    else if (node->left) son = node->left;
    else if (node->right) son = node->right;
    else return node->key;
 
    int aux = successor(key, son);
    if (aux < key || node->key < key)
        return std::max(aux, node->key);
 
    return std::min(aux, node->key);
}
 
template<class T>
T AVL<T>::successor(T key) {
    return successor(key, root);
}
 
template<class T>
void AVL<T>::print(T x, T y, Node <T> *node) {
    if (node->left && node->key >= x)
        print(x, y, node->left);
 
    if (node->key >= x && node->key <= y)
        fout << node->key << " "; 
 
    if (node->right && node->key <= y)
        print(x, y, node->right);
}
 
template<class T>
void AVL<T>::print(T x, T y) {
  print(x, y, root);
  fout << "\n";
}
 
 
int main()
{
    AVL<int> meow;
    int q;
    fin >> q;
 
    while (q--) {
        int op, x;
        fin >> op >> x;
 
        if (op == 1) meow.push(x);
        else if (op == 2) meow.erase(x);
        else if (op == 3) fout << meow.find(x) << '\n';
        else if (op == 4) fout << meow.predecessor(x) << '\n';
        else if (op == 5) fout << meow.successor(x) << '\n';
        else {
            int y;
            fin >> y;
            meow.print(x, y);
        }
    }
 
    return 0;
}

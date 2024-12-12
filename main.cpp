#include <iostream>
#include <queue>
using namespace std;

enum Color
{
    RED,
    BLACK
};

struct Node
{
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data)
    {
        this->data = data;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RBTree
{
private:
    Node *root;

    void rotateLeft(Node *&pt)
    {
        Node *pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr)
            pt->right->parent = pt;
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;
        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node *&pt)
    {
        Node *pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr)
            pt->left->parent = pt;
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(Node *&pt)
    {
        Node *parent_pt = nullptr;
        Node *grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) &&
               (pt->parent->color == RED))
        {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left)
            {
                Node *uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == RED)
                {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else
                {
                    if (pt == parent_pt->right)
                    {
                        rotateLeft(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
            else
            {
                Node *uncle_pt = grand_parent_pt->left;

                if ((uncle_pt != nullptr) && (uncle_pt->color == RED))
                {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else
                {
                    if (pt == parent_pt->left)
                    {
                        rotateRight(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    void fixDeletion(Node *x)
    {
        while (x != root && (x == nullptr || x->color == BLACK))
        {
            if (x && x->parent && x == x->parent->left)
            {
                Node *sibling = x->parent->right;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    sibling = x->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK))
                {
                    sibling->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (sibling->right == nullptr || sibling->right->color == BLACK)
                    {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = x->parent->right;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *sibling = x->parent->left;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    sibling = x->parent->left;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK))
                {
                    sibling->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (sibling->left == nullptr || sibling->left->color == BLACK)
                    {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = x->parent->left;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr)
            x->color = BLACK;
    }

    Node *deleteNodeHelper(Node *node, int data)
    {
        Node *z = nullptr;
        Node *x, *y;
        while (node != nullptr)
        {
            if (node->data == data)
                z = node;
            if (node->data <= data)
                node = node->right;
            else
                node = node->left;
        }

        if (z == nullptr)
        {
            cout << "Key not found in the tree" << endl;
            return root;
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == nullptr)
        {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == nullptr)
        {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else
        {
            y = minValueNode(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z)
            {
                if (x != nullptr)
                    x->parent = y;
            }
            else
            {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK)
            fixDeletion(x);
        return root;
    }

    void rbTransplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    Node *minValueNode(Node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

public:
    RBTree() { root = nullptr; }

    void insert(const int &data)
    {
        Node *pt = new Node(data);
        root = BSTInsert(root, pt);
        fixViolation(pt);
    }

    void deleteNode(int data)
    {
        root = deleteNodeHelper(root, data);
    }

    void inorder() { inorderHelper(root); }

    void levelOrder()
    {
        if (root == nullptr)
            return;
        queue<Node *> q;
        q.push(root);
        while (!q.empty())
        {
            int nodeCount = q.size();
            while (nodeCount > 0)
            {
                Node *temp = q.front();
                q.pop();
                cout << temp->data << "(" << (temp->color ? "BLACK" : "RED") << ") ";
                if (temp->left != nullptr)
                    q.push(temp->left);
                if (temp->right != nullptr)
                    q.push(temp->right);
                nodeCount--;
            }
            cout << endl;
        }
    }

private:
    void inorderHelper(Node *root)
    {
        if (root == nullptr)
            return;
        inorderHelper(root->left);
        cout << root->data << "(" << (root->color ? "BLACK" : "RED") << ") ";
        inorderHelper(root->right);
    }

    Node *BSTInsert(Node *root, Node *pt)
    {
        if (root == nullptr)
            return pt;
        if (pt->data < root->data)
        {
            root->left = BSTInsert(root->left, pt);
            root->left->parent = root;
        }
        else if (pt->data > root->data)
        {
            root->right = BSTInsert(root->right, pt);
            root->right->parent = root;
        }
        return root;
    }
};

int main()
{
    RBTree tree;
    tree.insert(41);
    tree.insert(38);
    tree.insert(31);
    tree.insert(12);
    tree.insert(19);
    tree.insert(8);
    tree.levelOrder();
    tree.deleteNode(8);
    cout << "----------\n";
    tree.levelOrder();
}
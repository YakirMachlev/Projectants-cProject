#include "avl_tree.h"

struct avl_node_t
{
    unsigned long key;
    void *data;
    avl_node_t *left;
    avl_node_t *right;
    int height;
};

unsigned long avl_tree_get_key(avl_node_t *node)
{
    return node->key;
}

void *avl_tree_get_data(avl_node_t *node)
{
    return node ? node->data : NULL;
}

avl_node_t *avl_tree_get_left_child(avl_node_t *node)
{
    return node->left;
}

avl_node_t *avl_tree_get_right_child(avl_node_t *node)
{
    return node->right;
}

int avl_tree_get_height(avl_node_t *node)
{
    if (!node)
        return 0;
    return node->height;
}

void avl_tree_set_key(avl_node_t *node, unsigned long key)
{
    node->key = key;
}

void avl_tree_set_data(avl_node_t *node, void *data)
{
    node->data = data;
}

/**
 * @brief determines which number is bigger between two given numbers
 *
 * @param num1 the first number
 * @param num2 the second number
 * @return int the bigger number
 */
static unsigned long avl_tree_max(unsigned long num1, unsigned long num2)
{
    return num1 > num2 ? num1 : num2;
}

/**
 * @brief rotates right the given node (used to balance the avl tree)
 *
 * @param root a node in the avl tree
 * @return Node* the node that is rotated right
 */
static void avl_tree_rotate_right(avl_node_t **root)
{
    avl_node_t *left_child;

    left_child = (*root)->left;
    (*root)->left = left_child->right;
    left_child->right = *root;

    (*root)->height = avl_tree_max(avl_tree_get_height((*root)->left), avl_tree_get_height((*root)->right)) + 1;
    left_child->height = avl_tree_max(avl_tree_get_height(left_child->left), avl_tree_get_height(left_child->right)) + 1;

    *root = left_child;
}

/**
 * @brief rotates left the given node (used to balance the avl tree)
 *
 * @param root a node in the avl tree
 * @return Node* the node that is rotated left
 */
static void avl_tree_rotate_left(avl_node_t **root)
{
    avl_node_t *right_child;

    right_child = (*root)->right;
    (*root)->right = right_child->left;
    right_child->left = *root;

    (*root)->height = avl_tree_max(avl_tree_get_height((*root)->left), avl_tree_get_height((*root)->right)) + 1;
    right_child->height = avl_tree_max(avl_tree_get_height(right_child->left), avl_tree_get_height(right_child->right)) + 1;

    *root = right_child;
}

/**
 * @brief calculates the balance factor of a given node
 *
 * @param n the node for which it finds the balance factor
 * @return int the balance factor of the node
 */
static int avl_tree_get_balance_factor(avl_node_t *node)
{
    if (!node)
        return 0;
    return avl_tree_get_height(node->left) - avl_tree_get_height(node->right);
}

avl_node_t *avl_tree_create_node(unsigned long key)
{
    avl_node_t *node;
    node = (avl_node_t *)malloc(sizeof(avl_node_t));
    assert("Couldn't allocate new node for the tree\n" && node != NULL);

    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->data = NULL;
    node->height = 1;

    return node;
}

void avl_tree_insert_node(avl_node_t **root, avl_node_t *node)
{
    if (!(*root))
        *root = node;
    else if (node->key > (*root)->key)
    {
        avl_tree_insert_node(&((*root)->right), node);
        if (avl_tree_get_balance_factor(*root) == -2)
        {
            if (node->key > (*root)->right->key)
                avl_tree_rotate_left(root);
            else
            {
                avl_tree_rotate_right(&((*root)->right));
                avl_tree_rotate_left(root);
            }
        }
    }
    else
    {
        avl_tree_insert_node(&((*root)->left), node);
        if (avl_tree_get_balance_factor(*root) == 2)
        {
            if (node->key < (*root)->left->key)
                avl_tree_rotate_right(&(*root));
            else
            {
                avl_tree_rotate_left(&((*root)->left));
                avl_tree_rotate_right(root);
            }
        }
    }
    (*root)->height = avl_tree_max(avl_tree_get_height((*root)->left), avl_tree_get_height((*root)->right)) + 1;
}

avl_node_t *avl_tree_search_node(avl_node_t *root, unsigned long key)
{
    if (!root)
        return NULL;

    else if (key < root->key)
        root = avl_tree_search_node(root->left, key);
    else if (key > root->key)
        root = avl_tree_search_node(root->right, key);

    return root;
}

avl_node_t *avl_tree_delete_node(avl_node_t *root, unsigned long key)
{
    avl_node_t *temp;

    if (!root)
        return root;

    if (key > root->key)
    {
        root->right = avl_tree_delete_node(root->right, key);
        if (avl_tree_get_balance_factor(root) == 2)
        {
            if (avl_tree_get_balance_factor(root->left) >= 0)
                avl_tree_rotate_right(&root);
            else
            {
                avl_tree_rotate_left(&(root->left));
                avl_tree_rotate_right(&root);
            }
        }
    }
    else if (key < root->key)
    {
        root->left = avl_tree_delete_node(root->left, key);
        if (avl_tree_get_balance_factor(root) == -2)
        {
            if (avl_tree_get_balance_factor(root->right) <= 0)
                avl_tree_rotate_left(&root);
            else
            {
                avl_tree_rotate_right(&(root->right));
                avl_tree_rotate_left(&root);
            }
        }
    }
    else
    {
        if (root->right)
        {
            temp = root->right;
            while (temp->left)
                temp = temp->left;

            root->key = temp->key;
            root->right = avl_tree_delete_node(root->right, temp->key);
            if (avl_tree_get_balance_factor(root) == 2)
            {
                if (avl_tree_get_balance_factor(root->left) >= 0)
                    avl_tree_rotate_right(&root);
                else
                {
                    avl_tree_rotate_left(&(root->left));
                    avl_tree_rotate_right(&root);
                }
            }
        }
        else
            return root->left;
    }
    root->height = avl_tree_max(avl_tree_get_height(root->left), avl_tree_get_height(root->right)) + 1;
    return root;
}

void avl_tree_destroy(avl_node_t *root)
{
    if (!root)
        return;

    avl_tree_destroy(root->left);
    avl_tree_destroy(root->right);
    free(root);
}

/**
 * @brief prints a given level's nodes of the avl tree
 *
 * @param root the root of the tree
 * @param level the number of the level that is printed
 */
static void avl_tree_print_level(avl_node_t *root, int level)
{
    if (!root)
    {
        printf("x  ");
        return;
    }
    if (level == 0)
        printf("y  ");
    else
    {
        avl_tree_print_level(root->left, level - 1);
        avl_tree_print_level(root->right, level - 1);
    }
}

void avl_tree_print_level_order(avl_node_t *root)
{
    int height;
    int level;

    height = avl_tree_get_height(root);
    for (level = 0; level < height; level++)
    {
        avl_tree_print_level(root, level);
        printf("\n");
    }
    printf("\n");
}

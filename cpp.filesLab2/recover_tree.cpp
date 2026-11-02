#include "../h.filesLab2/recover_tree.h"
#include <string>
#include <algorithm>

using namespace std;

static void inorder_find(TreeNode* curr, TreeNode*& prev, TreeNode*& first, TreeNode*& second) {
    if (curr == nullptr) return;

    inorder_find(curr->left, prev, first, second);

    if (prev != nullptr) {
        int prev_val = stoi(prev->val);
        int curr_val = stoi(curr->val);
        if (prev_val > curr_val) {
            if (first == nullptr) {
                first = prev;
            }
            second = curr;
        }
    }
    prev = curr;

    inorder_find(curr->right, prev, first, second);
}

void recover_tree(TreeNode* root) {
    TreeNode* prev = nullptr;
    TreeNode* first = nullptr;
    TreeNode* second = nullptr;

    inorder_find(root, prev, first, second);

    if (first != nullptr && second != nullptr) {
        swap(first->val, second->val);
    }
}
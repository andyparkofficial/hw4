#include "equal-paths.h"
#include <algorithm>
using namespace std;


// You may add any prototypes of helper functions here
int height(Node * root){
  if (root == nullptr) {
    return 0;
  }
  else {
    return 1 + max(height(root->left), height(root->right));
  }
}

int balance(Node * root) {
  int leftHeight = height(root->left);
  int rightHeight = height(root->right);
  return (leftHeight-rightHeight);
}

bool equalPaths(Node * root)
{
  // if root is leaf node
    if (root == nullptr) {
      return true;
    }
    // if left and right children exist
    else if (root->left != nullptr && root->right != nullptr){
      if (height(root->left) == height(root->right)){ // if both heights are the same
        if (equalPaths(root->left) && equalPaths(root->right)) {
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else if (root->left != nullptr){ // only left exists
      return equalPaths(root->left);
    } else{ // only right exists
      return equalPaths(root->right);
    } 
}


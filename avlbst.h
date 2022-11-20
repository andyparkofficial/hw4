#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix (AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix (AVLNode<Key,Value>* n, int diff);
    void rotateRight (AVLNode<Key,Value>* rotatingNode);
    void rotateLeft (AVLNode<Key,Value>* rotatingNode);


};


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{

  if (p == NULL){ // if parent is NULL
    return;
  } else if (p->getParent() == NULL){ // if grandparent is NULL
    return;
  } else { 
    AVLNode<Key, Value>* g = p->getParent();
    
    if (g->getLeft() == p){ // if p is left child of g
      
      g->updateBalance(-1);
      
      // Case 1:
      if (g->getBalance() == 0){
        return;

      // Case 2:
      } else if (g->getBalance() == -1){
        insertFix(g, p);

      // Case 3:
      } else { // g->getBalance() == -2
        if (p->getLeft() == n){ // if zig-zig
          this->rotateRight(g);
          g->setBalance(0);
          p->setBalance(0);
        } else { // if zig-zag
          this->rotateLeft(p);
          this->rotateRight(g);



          // Case 3a:
          if (n->getBalance() == -1){
            p->setBalance(0);
            g->setBalance(1);
            n->setBalance(0);


          // Case 3b:
          } else if (n->getBalance() == 0){
            p->setBalance(0);
            g->setBalance(0);
            n->setBalance(0);


          // Case 3c:
          } else if (n->getBalance() == 1){ // n->getBalance() == 1
            p->setBalance(-1);
            g->setBalance(0);
            n->setBalance(0);

          }
        }
      }

    } else { // if p is right child of g

      g->updateBalance(1);
      
      // Case 1:
      if (g->getBalance() == 0){
        return;

      // Case 2:
      } else if (g->getBalance() == 1){
        insertFix(g, p);

      // Case 3:
      } else { // g->getBalance() == 2
        if (p->getRight() == n){ // if zig-zig
          this->rotateLeft(g);
          g->setBalance(0);
          p->setBalance(0);
        } else { // if zig-zag
          this->rotateRight(p);
          this->rotateLeft(g);

          
          // Case 3a:
          if (n->getBalance() == 1){
            p->setBalance(0);
            g->setBalance(-1);
            n->setBalance(0);


          // Case 3b:
          } else if (n->getBalance() == 0){
            p->setBalance(0);
            g->setBalance(0);
            n->setBalance(0);

          // Case 3c:
          } else if (n->getBalance() == -1){ // n->getBalance() == -1
            p->setBalance(1);
            g->setBalance(0);
            n->setBalance(0);

          }
        }
      }
    }

  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if (this->empty()){ // if tree is empty
      AVLNode<Key, Value>* insertedNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      insertedNode->setBalance(0);
      this->root_ = insertedNode;
      
    } else if (this->internalFind(new_item.first) != NULL) { // tree has same key
      (this->internalFind(new_item.first))->setValue(new_item.second);
    } else { // tree is not empty and doesn't have same key

      AVLNode<Key, Value>* tempNode = (AVLNode<Key, Value>*)this->root_;
      AVLNode<Key, Value>* parentNode = NULL;
      bool isRightChild = false; // false = is left child; true = is right child

      while (tempNode != NULL) {
        if (tempNode->getKey() < new_item.first) { // if key is greater than tempNode key
          parentNode = tempNode;
          tempNode = tempNode->getRight();
          isRightChild = true;
        } else if (tempNode->getKey() > new_item.first){ // if key is less than tempNode key
          parentNode = tempNode;
          tempNode = tempNode->getLeft();
          isRightChild = false;
        }
      }

      AVLNode<Key, Value>* insertedNode = new AVLNode <Key, Value>(new_item.first, new_item.second, NULL);

      if (isRightChild){ // if insertedNode is a right child
        parentNode->setRight(insertedNode);
        insertedNode->setParent(parentNode);

      } else { // if insertedNode is a left child
        parentNode->setLeft(insertedNode);
        insertedNode->setParent(parentNode);
      }

      if (parentNode->getBalance() == -1 ){
        parentNode->setBalance(0);
      } else if (parentNode->getBalance() == 1 ){
        parentNode->setBalance(0);
      } else { // parent's balance is 0
        if (isRightChild){
          parentNode->setBalance(1);
        } else {
          parentNode->setBalance(-1);
        }

        this->insertFix(parentNode, insertedNode);
      }


      
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key,Value>* rotatingNode){

  if (rotatingNode == this->root_){ // if rotatingNode is root
    AVLNode<Key,Value>* leftChild = rotatingNode->getLeft();

    this->root_ = leftChild;
    leftChild->setParent(NULL);
    rotatingNode->setParent(leftChild);
    rotatingNode->setLeft(leftChild->getRight());
    if (leftChild->getRight() != NULL){
      (leftChild->getRight())->setParent(rotatingNode);
    }
    leftChild->setRight(rotatingNode);
  } else {

    AVLNode<Key,Value>* leftChild = rotatingNode->getLeft();
    AVLNode<Key,Value>* rotatingParent = rotatingNode->getParent();
    
    if (rotatingParent != NULL){
      if (rotatingParent->getLeft() == rotatingNode){ // if rotatingNode is left child of rotatingParent
        rotatingParent->setLeft(leftChild);
        leftChild->setParent(rotatingParent);
      } else { // if rotatingNode is right child of rotatingParent
        rotatingParent->setRight(leftChild);
        leftChild->setParent(rotatingParent);
      }
    
    
    } else {
      leftChild->setParent(NULL);
    }
    
    rotatingNode->setLeft(leftChild->getRight());
    if (leftChild->getRight() != NULL){
      (leftChild->getRight())->setParent(rotatingNode);
    }
    leftChild->setRight(rotatingNode);
    rotatingNode->setParent(leftChild);

    
  }


}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key,Value>* rotatingNode){
  if (rotatingNode == this->root_){ // if rotatingNode is root
    AVLNode<Key,Value>* rightChild = rotatingNode->getRight();

    this->root_ = rightChild;
    rightChild->setParent(NULL);
    rotatingNode->setParent(rightChild);
    rotatingNode->setRight(rightChild->getLeft());
    if (rightChild->getLeft() != NULL){
      (rightChild->getLeft())->setParent(rotatingNode);
    }
    rightChild->setLeft(rotatingNode);
  } else {


    AVLNode<Key,Value>* rightChild = rotatingNode->getRight();
    AVLNode<Key,Value>* rotatingParent = rotatingNode->getParent();
    
    if (rotatingParent != NULL){
      if (rotatingParent->getLeft() == rotatingNode){ // if rotatingNode is left child of rotatingParent
        rotatingParent->setLeft(rightChild);
        rightChild->setParent(rotatingParent);
      } else { // if rotatingNode is right child of rotatingParent
        rotatingParent->setRight(rightChild);
        rightChild->setParent(rotatingParent);
      }   
    } else {
      rightChild->setParent(NULL);
    }
    rotatingNode->setRight(rightChild->getLeft());
    if (rightChild->getLeft() != NULL){
      (rightChild->getLeft())->setParent(rotatingNode);
    }
    rightChild->setLeft(rotatingNode);
    rotatingNode->setParent(rightChild);

  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix (AVLNode<Key,Value>* n, int diff)
{
  if (n == NULL) return;

  AVLNode<Key, Value>* p = n->getParent();
  int ndiff = 0;

  if (p != NULL){
    if (p->getLeft() == n){ // if n is left child
      ndiff = 1;
    } else { // if n is right child
      ndiff = -1;
    }
  }

  if (diff == -1){

    // Case 1: b(n) + diff == -2
    if ((n->getBalance()+ diff) == -2){
      AVLNode<Key, Value>* c = n->getLeft();

      // Case 1a: b(c) == -1 zig-zig case
      if (c->getBalance() == -1){
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);

      // Case 1b: b(c) == 0 zig-zig cas
      } else if(c->getBalance() == 0){
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);

      // Case 1c: b(c) == +1 zig-zag case
      } else {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if (g->getBalance() == 1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);

        } 
        else if (g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);

        } 
        else  { // g->getBalance() == -1
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);

        }
        removeFix(p, ndiff);
      }
      
    // Case 2: b(n) + diff == -1
    } 
    else if ((n->getBalance()+ diff) == -1){
      n->setBalance(-1);

    // Case 3: b(n) + diff == 0
    } 
    else if ((n->getBalance()+ diff) == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  } 
  else if (diff == 1){ // diff == 1

    // Case 1: b(n) + diff == 2
    if ((n->getBalance()+ diff) == 2){
      AVLNode<Key, Value>* c = n->getRight();

      // Case 1a: b(c) == 1 zig-zig case
      if (c->getBalance() == 1){
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);

      // Case 1b: b(c) == 0 zig-zig cas
      }
      else if(c->getBalance() == 0){
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);

      // Case 1c: b(c) == -1 zig-zag case
      }
      else {
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if (g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);

        } else if (g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);

        } else { // g->getBalance() == 1
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);

        }
        removeFix(p, ndiff);
      }
      
    // Case 2: b(n) + diff == 1
    } 
    else if ((n->getBalance()+ diff) == 1){
      n->setBalance(1);

    // Case 3: b(n) + diff == 0
    } 
    else if ((n->getBalance()+ diff) == 0) {
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO


    // BC: node with key doesnt exist
    // first find the node with the key 
    AVLNode<Key, Value>* removingNode = (AVLNode<Key, Value>*)this->internalFind(key);
    if (removingNode == NULL){
      return;
    }
    
    else { // removingNode exists


      // Case: 2 children: Swap the value with its in-order predecessor and then
      // remove from its new location
      if (removingNode->getLeft() != NULL && removingNode->getRight() != NULL){
        AVLNode<Key, Value>* predecessor = (AVLNode<Key, Value>*)BinarySearchTree<Key, Value>::predecessor(removingNode);
        this->nodeSwap(predecessor, removingNode);
      }
      
      int diff;
      AVLNode<Key, Value>* parentNode = removingNode->getParent();
      if (parentNode != NULL){
        if (removingNode == parentNode->getLeft()){ // if removingNode is left child
          diff = 1;
        } else { // if removingNode is right child
          diff = -1;
        }
      }


      // Case: 0 children: If the value is in a leaf node, simply remove that leaf node
      if (removingNode->getLeft() == NULL && removingNode->getRight() == NULL){ 

        if (removingNode->getParent() != NULL){ // if parent exists
          if ((removingNode->getParent())->getRight() == removingNode){ // if removingNode is rightChild
            (removingNode->getParent())->setRight(NULL); // set parent's right child NULL to remove

          } else { // if removingNode is leftChild
            (removingNode->getParent())->setLeft(NULL); // set parent's left child NULL to remove
          }
          delete removingNode;

        } else { // if parent doesn't exist
          this->clear();
          return;
        }
        
      }


      // Case: 1 child: Promote the child into the node's location
      else if ((removingNode->getLeft() == NULL && removingNode->getRight() != NULL) || (removingNode->getLeft() != NULL && removingNode->getRight() == NULL)) {
        
        if (removingNode->getLeft() != NULL) { // if left child exists

          if (removingNode->getParent() == NULL) { // if parentNode doesn't exist; removingNode is root
            (removingNode->getLeft())->setParent(NULL);
            this->root_ = removingNode->getLeft();
            
          } else {
            if ((removingNode->getParent())->getRight() == removingNode){ // if removingNode is rightChild
              (removingNode->getParent())->setRight(removingNode->getLeft());
              (removingNode->getLeft())->setParent(removingNode->getParent());
            } else { // if removingNode is leftChild
              (removingNode->getParent())->setLeft(removingNode->getLeft());
              (removingNode->getLeft())->setParent(removingNode->getParent());
            }
          }
          

        } else { // if right child exists

          if (removingNode->getParent() == NULL) { // if parentNode doesn't exist; removingNode is root
            (removingNode->getRight())->setParent(NULL);
            this->root_ = removingNode->getRight();
            
          } else {
            if ((removingNode->getParent())->getRight() == removingNode){ // if removingNode is rightChild
              (removingNode->getParent())->setRight(removingNode->getRight());
              (removingNode->getRight())->setParent(removingNode->getParent());
            } else { // if removingNode is leftChild
              (removingNode->getParent())->setLeft(removingNode->getRight());
              (removingNode->getRight())->setParent(removingNode->getParent());
            }
          }
        }
        delete removingNode;
      }
    
      this->removeFix(parentNode, diff);
      
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

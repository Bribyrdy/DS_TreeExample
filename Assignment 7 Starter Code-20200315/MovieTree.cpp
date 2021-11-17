#include <iostream>
#include <fstream>
#include "MovieTree.hpp"
using namespace std;

void destroyRec(TreeNode* node)
{
    if (node == NULL) {return;}
    if(node)
    {
        destroyRec(node->leftChild);
        destroyRec(node->rightChild);
    }
    if (node->head != NULL)
    {
        LLMovieNode* llNode;
        while (node->head != NULL)
        {
            llNode = node->head;
            node->head = node->head->next;
            delete llNode;
        }
        node->head = NULL; 
    }
    delete node; 
}

void printHelper(TreeNode* node)
{ 
    if (node == NULL) 
        return;
    printHelper(node->leftChild);
    cout << "Movies starting with letter: " << node->titleChar << endl;
    for (LLMovieNode* nn = node->head; nn != 0; nn = nn->next)
    {
        cout << " >> " << nn->title << " " << nn->rating << endl;
    }
    printHelper(node->rightChild);
}
LLMovieNode* searchList(LLMovieNode* curr, string key) {
    if (curr == NULL) return curr;
    while (curr != NULL && curr->title != key)
    {
        curr = curr->next;
    }
    return curr;
}
TreeNode* getMinValueNode(TreeNode* currNode){

    if(currNode->leftChild == NULL){
      return currNode;
    }
    return getMinValueNode(currNode->leftChild);
}

/* ------------------------------------------------------ */

MovieTree::MovieTree()
{
	root = NULL;
}

MovieTree::~MovieTree()
{
    destroyRec(root);
}

void MovieTree::printMovieInventory()
{
    if (root == NULL)
    {
        return;
    }
    else 
    {
        printHelper(root); 
    }
}

void MovieTree::addMovie(int ranking, string title, int year, float rating)
{
    TreeNode* node = root;
    TreeNode* Nparent = NULL; 
    TreeNode* whichTreeLetter = searchChar(title[0]);
    TreeNode* newN = new TreeNode; 
    newN->titleChar = title[0]; 
    
    if (whichTreeLetter == NULL)
    {
        while (node != NULL)
        {
            Nparent = node; 
            if (newN->titleChar < node->titleChar)
            {
                node = node->leftChild; 
            }
            else 
            {
                node = node->rightChild; 
            }
        }
        if (Nparent == NULL) {root = newN;}
        else if(newN->titleChar < Nparent->titleChar)
        {
            Nparent->leftChild = newN; 
            newN->parent = Nparent;
        }
        else 
        {
            Nparent->rightChild = newN; 
            newN->parent = Nparent; 
        }
        LLMovieNode* llNode = new LLMovieNode(ranking, title, year, rating);
        newN->head = llNode; 
        return;
    }
    else
    {
        LLMovieNode* newLL = new LLMovieNode(ranking, title, year, rating);
        LLMovieNode* test = whichTreeLetter->head;
        LLMovieNode* test2 = whichTreeLetter->head->next;
        while (test != NULL)
        { 
            if (test->title > title)
            {
                if (test == whichTreeLetter->head)
                {
                    newLL->next = whichTreeLetter->head;
                    whichTreeLetter->head = newLL;
                    delete newN;
                    return;
                }
                while (test2->next != test)
                {
                    test2 = test2->next; 
                }
                test2->next = newLL;
                newLL->next = test;
                delete newN;
                return;
            }
            test = test->next;
        }
        while (whichTreeLetter->head->next != NULL)
        {
            whichTreeLetter->head = whichTreeLetter->head->next;
        }
        whichTreeLetter->head->next = newLL;
        delete newN;
    }
}

void MovieTree::deleteMovie(std::string title)
{
    if (root == NULL) // if the tree is empty 
    {
        cout <<  "Movie: " << title << " not found, cannot delete." << endl;
    }
    TreeNode* nn = searchChar(title[0]); // tree node deleting from
    LLMovieNode* nnHead = nn->head; // tree nodes linked list pointer 
    LLMovieNode* theNode = searchList(nnHead, title); //returns the node in LL
    bool check = false;
    bool found = false; 
    bool flag = false;
    if (nnHead == theNode && nnHead->next == NULL) // if the linked list only has theNode
    {
        nnHead = nnHead->next;
        delete theNode; 
        check = true; 
        flag = false; 
    }
    else if (nnHead == theNode && nnHead->next != NULL)
    {
        found = true; 
        nn->head = nn->head->next;
        delete nnHead;
        flag = true; 
    }
    else  // if the list has more than just the node 
    {
        LLMovieNode* nodeNext = nnHead->next; 
        while(nnHead != NULL && found != true) // found != true
        {
            if (nodeNext->title == title)
            {
                if (nodeNext->next == 0)
                {
                    nnHead->next = 0;
                    nodeNext->next = nnHead; 
                }
                else 
                {
                    nnHead->next = nodeNext->next;
                }
                found = true; 
            }
            else 
            {
                nnHead = nodeNext;
                nodeNext = nodeNext->next; 
            }
        }
        delete nodeNext;
    }
    if (check == true && flag == false)
    {
        if (nn != root)
        {
            if (nn->leftChild == NULL && nn->rightChild == NULL)
            {
                if (nn->parent->leftChild == nn)
                {
                    nn->parent->leftChild = NULL;
                }
                else if (nn->parent->rightChild == nn)
                {
                    nn->parent->rightChild = NULL; 
                }
                delete nn;
                return; 
            }
            else if (nn->leftChild != NULL && nn->rightChild != NULL)
            {
                TreeNode* min = getMinValueNode(nn->rightChild); 
                TreeNode* succ = min;
                deleteMovie(nn->head->title); 
                if (nn->parent != NULL)
                {
                    if (nn == nn->parent->leftChild)
                    {
                        nn->parent->leftChild = succ;
                    }
                    else 
                    {
                        nn->parent->rightChild = succ; 
                    }
                }
                else 
                {
                    root = succ;
                }
                succ->leftChild = nn->leftChild;
                succ->rightChild = nn->rightChild;
                succ->parent = nn->parent; 
            }
            else // if there's one child
            {
                TreeNode* l = new TreeNode;
                if (nn->leftChild != NULL)
                {
                    l = nn->leftChild;
                }
                else 
                {
                    l = nn->rightChild; 
                }
                if (nn->parent->leftChild == nn)
                {
                    nn->parent->leftChild = l; 
                }
                else 
                {
                    nn->parent->rightChild = l; 
                }
                l->parent = nn->parent;
                delete nn; 
            }
        }
        else 
        {
            TreeNode* delPtr = root;
            if (root-> rightChild == NULL && root->leftChild == NULL)
            {
                root = NULL;
            }
            else if (root->leftChild == NULL && root->rightChild != NULL)
            {
                root = root->rightChild; 
                delPtr->rightChild = NULL; 
            }
            else if (root->leftChild != NULL && root->rightChild == NULL)
            {
                root = root->leftChild;
                delPtr->leftChild = NULL;
            }
            else 
            {
                TreeNode* min = getMinValueNode(root->rightChild);
                root->head = min->head; 
                deleteMovie(min->head->title);
                root->titleChar = min->titleChar;
            }
            delete delPtr;
        }
    }
}

void MovieTree::leftRotation(TreeNode* curr)
{
    TreeNode* newNode = curr->rightChild;
    curr->rightChild = newNode->leftChild; 
    if (newNode->leftChild != NULL)
    {
        newNode->leftChild->parent = curr;
    }
    newNode->parent = curr->parent;
    
    if (curr->parent == NULL)
    {
        root = newNode;
    }
    else 
    {
        if (curr == curr->parent->leftChild)
        {
            curr->parent->leftChild = newNode;
        }
        else 
        {
            curr->parent->rightChild = newNode;
        }
    }
    newNode->leftChild = curr;
    curr->parent = newNode;
    delete newNode; 
}

//------ Given Methods--------//
void _grader_inorderTraversal(TreeNode * root)
{
	if (root == NULL)
		return;
	_grader_inorderTraversal(root->leftChild);
	cout << root->titleChar << " ";
	_grader_inorderTraversal(root->rightChild);
}

void MovieTree::inorderTraversal() {
	_grader_inorderTraversal(root);
}

TreeNode* searchCharHelper(TreeNode* curr, char key)
{
    if (curr == NULL)
        return curr;
    else if(curr->titleChar == key)
        return curr;
    else if (curr->titleChar > key)
        return searchCharHelper(curr->leftChild, key);
    else
        return searchCharHelper(curr->rightChild, key);
}

TreeNode* MovieTree::searchChar(char key)
{
    return searchCharHelper(root, key);
}


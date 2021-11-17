#include <iostream>
#include <fstream>
#include "MovieTree.hpp"

using namespace std;

void destroyRec(TreeNode* node)
{
    if(node)
    {
        destroyRec(node->leftChild);
        destroyRec(node->rightChild);
        delete node;
    }
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
    LLMovieNode* ptr = curr;
    while (ptr != NULL && ptr->title != key)
    {
        ptr = ptr->next;
    }
    return ptr;
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
    cout << "Constructor was called" << endl; 
	root = NULL;
}

MovieTree::~MovieTree()
{
    destroyRec(root); 
    cout << "destructor was called" << endl; 
}

void MovieTree::printMovieInventory()
{
    cout << "print movie was called " << endl;
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
    cout << "addmove was called" << endl; 
    TreeNode* isNull = NULL;
    TreeNode* node = root;
    TreeNode* whichTreeLetter = searchChar(title[0]);
    TreeNode* newN = new TreeNode; 
    LLMovieNode* llNode = new LLMovieNode(ranking, title, year, rating); 
    
    if (whichTreeLetter == NULL)
    {
        newN->titleChar = title[0]; 
        while (node != NULL)
        {
            isNull = node; 
            if (newN->titleChar < node->titleChar)
            {
                node = node->leftChild; 
            }
            else 
            {
                node = node->rightChild; 
            }
        }
        
        if (isNull == NULL) {root = newN;}
        else if(newN->titleChar < isNull->titleChar)
        {
            isNull->leftChild = newN; 
            newN->parent = isNull;
        }
        else 
        {
            isNull->rightChild = newN; 
            newN->parent = isNull; 
        }
        newN->head = llNode; 

        return;
    }
        LLMovieNode* test = whichTreeLetter->head;
        LLMovieNode* test3 = whichTreeLetter->head; 
        LLMovieNode* test2 = whichTreeLetter->head->next;
        while (test != NULL)
        { 
            if (test->title > title)
            {
                if (test == whichTreeLetter->head)
                {
                    llNode->next = whichTreeLetter->head;
                    whichTreeLetter->head = llNode; 
                    return;
                }
                while (test2->next != test)
                {
                    test2 = test2->next; 
                }
                test2->next = llNode;
                llNode->next = test;
                return;
            }
            test = test->next;
        }
        while (test3->next != NULL)
        {
            test3 = test3->next;
        }
        test3->next = llNode;
}

void MovieTree::deleteMovie(std::string title)
{
    cout << "deleteMovie called" << endl; 
    if (root == NULL) // if the tree is empty 
    {
        cout <<  "Movie: " << title << " not found, cannot delete." << endl;
        return;
    }
    TreeNode* nn = searchChar(title[0]); // tree node deleting from
    LLMovieNode* nnHead = nn->head; // tree nodes linked list pointer 
    LLMovieNode* theNode = searchList(nnHead, title); //returns the node in LL
    bool check = false;
    bool found = false; 
    bool flag = false; 
    if (nnHead == theNode && nnHead->next == NULL) // if the linked list only has theNode
    {
        LLMovieNode* node = nnHead;
        nnHead = nnHead->next; 
        check = true; 
        flag = false; 
    }
    else if (nnHead == theNode && nnHead->next != NULL)
    {
        LLMovieNode* node = nn->head;
        found = true; 
        nn->head = nn->head->next;
        delete node; 
        flag = true; 
    }
    else  // if the list has more than just the node 
    {
        LLMovieNode* node = nnHead; 
        LLMovieNode* nodeNext = node->next; 
        while(node != NULL && found != true) // found != true
        {
            if (nodeNext->title == title)
            {
                if (nodeNext->next == 0)
                {
                    node->next = 0;
                    nodeNext->next = node; 
                }
                else 
                {
                    node->next = nodeNext->next;
                }
                delete nodeNext; 
                found = true; 
            }
            else 
            {
                node = nodeNext;
                nodeNext = nodeNext->next; 
            }
        }
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
                delete nn;
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
            }
            delete nn;
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
                delete delPtr; 
            }
            else if (root->leftChild != NULL && root->rightChild == NULL)
            {
                root = root->leftChild;
                delPtr->leftChild = NULL;
                delete delPtr;
            }
            else 
            {
                TreeNode* min = getMinValueNode(root->rightChild);
                root->head = min->head; 
                deleteMovie(min->head->title); 
                root->titleChar = min->titleChar;
                
            }
        }
    }
}
void MovieTree::leftRotation(TreeNode* curr)
{
    cout << "left called" << endl; 
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
}
//------ Given Methods--------//
void _grader_inorderTraversal(TreeNode * root)
{
	if (root == NULL) {
		return;
	}

	_grader_inorderTraversal(root->leftChild);
	cout << root->titleChar << " ";
	_grader_inorderTraversal(root->rightChild);
}

void MovieTree::inorderTraversal() {
	_grader_inorderTraversal(root);
	cout << endl;
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

#include <cstring>
class BinaryTree
{
    public:
        BinaryTree(int freq, char val) : frequency(freq), value(val), leftChild (NULL), rightChild(NULL){ encoding = new char[128];};
        BinaryTree(int freq, const BinaryTree * tree1, const BinaryTree * tree2) : frequency(freq), value(0), leftChild (tree1), rightChild(tree2) {encoding = new char['a'];};
        void setLeftChild(const BinaryTree * tree) {leftChild = tree;};
        void setRightChild(const BinaryTree * tree) {rightChild = tree;};
        const BinaryTree* getLeftChild() const {return leftChild;};
        const BinaryTree* getRightChild() const {return rightChild;};
        int getFrequency() const {return frequency;};
        char getValue() const {return value;};
        void printTree(const BinaryTree * tree) const //inorder traversal
        {
            //encoding[strlen(encoding)-1] = '\0';
            if (!tree)
            {
                encoding[strlen(encoding)-1] = '\0';
                return ;
            }
            strcat(encoding, "0");
            printTree(tree->getLeftChild());
            if (tree->getValue() != 0) 
                std::cout << tree->getValue() << "    "<< tree->getFrequency() << " " <<encoding<<std::endl;
            strcat(encoding, "1");
            printTree(tree->getRightChild());
            
            encoding[strlen(encoding)-1] = '\0';
        }
    private:
        int frequency; 
        char value; 
        const BinaryTree* leftChild; 
        const BinaryTree* rightChild; 
        char *encoding;
};

class BinaryTreePtr
{
    public:
        BinaryTreePtr(){};
        BinaryTreePtr(const BinaryTree* tree) : currentTree(tree), treeLeft(tree->getLeftChild()), treeRight(tree->getRightChild()){};
        const BinaryTree* getCurrentTree() const {return currentTree;};
        const BinaryTree* getRightTree() const {return treeRight;};
        const BinaryTree* getLeftTree() const {return treeLeft;};
        bool operator<(const BinaryTreePtr& b) const { return currentTree->getFrequency() < b.getCurrentTree()->getFrequency(); }
    private:
        const BinaryTree *  currentTree; 
        const BinaryTree * treeLeft; 
        const BinaryTree * treeRight;
};
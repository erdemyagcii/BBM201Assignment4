#include <iostream>
#include <sstream>
#include <fstream>
#include<algorithm>
using namespace std;

struct SecondaryTree{
    string categoryName = "";
    string productName = "";
    int data = 0;
    struct SecondaryTree* leftChild = nullptr;
    struct SecondaryTree* rightChild = nullptr;
    int balanceFactor = 0;
    int height = 0;
};

struct PrimaryTree{
    string categoryName = "";
    struct PrimaryTree* leftChild = nullptr;
    struct PrimaryTree* rightChild = nullptr;
    struct SecondaryTree* firstNodeOfSecondaryTree = nullptr;
    bool isExist = true;
    int height = 0;
};

typedef struct PrimaryTree PrimaryTree;
typedef struct SecondaryTree SecondaryTree;

void findHeightsOfAVL(SecondaryTree* firstNodeOfSecondaryTree);
void findHeightsOfPrimaryTree(PrimaryTree* firstNodeOfPrimaryTree);
void calculateBalanceFactors(SecondaryTree* firstNodeOfSecondaryTree);
void setBalanceSecondaryTreeAVL(SecondaryTree* firstNodeOfSecondaryTree, SecondaryTree* const constantFirstNodeOfSecondaryTree, PrimaryTree* topTree);
void insertAVL(string category, string name, int price, PrimaryTree* firstNodeOfPrimaryTree);
void removeAVL(string category, string name, PrimaryTree* firstNodeOfPrimaryTree);
void printLevel(SecondaryTree* firstNodeOfSecondaryTree, int initialLevel);
void printAllItemsInCategory(string category, PrimaryTree* firstNodeOfPrimaryTree);
void printPrimaryTreesLevel(PrimaryTree* firstNodeOfPrimaryTree, int initialLevel);
void printAllItems(PrimaryTree* firstNodeOfPrimaryTree, int height);
void printItem(string category, string name, PrimaryTree* firstNodeOfPrimaryTree);
void findProduct(string category, string name, PrimaryTree* firstNodeOfPrimaryTree);
void updateData(string category, string name, int newData, PrimaryTree* firstNodeOfPrimaryTree);




ofstream writeToOutputFile;
int main(int argc, char** argv) {
    PrimaryTree* root = new PrimaryTree;
    root->isExist = false;
    string inputFilePath = argv[1];
    string outputFilePath = argv[2];
    writeToOutputFile.open(outputFilePath, std::ios_base::app);
    ifstream readInput(inputFilePath);
    string line;
    while(getline(readInput, line)){
        string arr[4];
        istringstream ss(line);
        string del;
        int i = 0;
        while(getline(ss, del, '\t')){
            arr[i] = del;
            i++;
        }
        if(arr[0] == "insert"){
            int price = atoi(arr[3].c_str());
            insertAVL(arr[1], arr[2], price, root);
        }
        else if(arr[0] == "remove")
            removeAVL(arr[1], arr[2], root);
        else if(arr[0] == "printAllItemsInCategory")
            printAllItemsInCategory(arr[1], root);
        else if(arr[0] == "printAllItems"){
            findHeightsOfPrimaryTree(root);
            printAllItems(root,root->height);
        }
        else if(arr[0] == "printItem")
            printItem(arr[1], arr[2], root);
        else if(arr[0] == "find")
            findProduct(arr[1], arr[2], root);
        else if(arr[0] == "updateData"){
            int price = atoi(arr[3].c_str());
            updateData(arr[1], arr[2], price, root);
        }
    }
    return 0;
}




void findHeightsOfAVL(SecondaryTree* firstNodeOfSecondaryTree){
    if(firstNodeOfSecondaryTree->leftChild == nullptr && firstNodeOfSecondaryTree->rightChild == nullptr)
        firstNodeOfSecondaryTree->height = 0;
    else if(firstNodeOfSecondaryTree->leftChild == nullptr && firstNodeOfSecondaryTree->rightChild != nullptr) {
        findHeightsOfAVL(firstNodeOfSecondaryTree->rightChild);
        firstNodeOfSecondaryTree->height = firstNodeOfSecondaryTree->rightChild->height + 1;
    }
    else if(firstNodeOfSecondaryTree->leftChild != nullptr && firstNodeOfSecondaryTree->rightChild == nullptr){
        findHeightsOfAVL(firstNodeOfSecondaryTree->leftChild);
        firstNodeOfSecondaryTree->height = firstNodeOfSecondaryTree->leftChild->height + 1;
    }
    else {
        findHeightsOfAVL(firstNodeOfSecondaryTree->leftChild);
        findHeightsOfAVL(firstNodeOfSecondaryTree->rightChild);
        firstNodeOfSecondaryTree->height = max(firstNodeOfSecondaryTree->leftChild->height, firstNodeOfSecondaryTree->rightChild->height) + 1;
    }
}

void findHeightsOfPrimaryTree(PrimaryTree* firstNodeOfPrimaryTree){
    if(firstNodeOfPrimaryTree->leftChild == nullptr && firstNodeOfPrimaryTree->rightChild == nullptr)
        firstNodeOfPrimaryTree->height = 0;
    else if(firstNodeOfPrimaryTree->leftChild == nullptr && firstNodeOfPrimaryTree->rightChild != nullptr) {
        findHeightsOfPrimaryTree(firstNodeOfPrimaryTree->rightChild);
        firstNodeOfPrimaryTree->height = firstNodeOfPrimaryTree->rightChild->height + 1;
    }
    else if(firstNodeOfPrimaryTree->leftChild != nullptr && firstNodeOfPrimaryTree->rightChild == nullptr){
        findHeightsOfPrimaryTree(firstNodeOfPrimaryTree->leftChild);
        firstNodeOfPrimaryTree->height = firstNodeOfPrimaryTree->leftChild->height + 1;
    }
    else {
        findHeightsOfPrimaryTree(firstNodeOfPrimaryTree->leftChild);
        findHeightsOfPrimaryTree(firstNodeOfPrimaryTree->rightChild);
        firstNodeOfPrimaryTree->height = max(firstNodeOfPrimaryTree->leftChild->height, firstNodeOfPrimaryTree->rightChild->height) + 1;
    }
}


void calculateBalanceFactors(SecondaryTree* firstNodeOfSecondaryTree){

    if(firstNodeOfSecondaryTree->leftChild != nullptr)
        calculateBalanceFactors(firstNodeOfSecondaryTree->leftChild);
    if(firstNodeOfSecondaryTree->rightChild != nullptr)
        calculateBalanceFactors(firstNodeOfSecondaryTree->rightChild);


    if(firstNodeOfSecondaryTree->leftChild == nullptr && firstNodeOfSecondaryTree->rightChild == nullptr)
        firstNodeOfSecondaryTree->balanceFactor = 0;
    else if(firstNodeOfSecondaryTree->leftChild != nullptr && firstNodeOfSecondaryTree->rightChild == nullptr)
        firstNodeOfSecondaryTree->balanceFactor = -((firstNodeOfSecondaryTree->leftChild->height) + 1);
    else if(firstNodeOfSecondaryTree->rightChild != nullptr && firstNodeOfSecondaryTree->leftChild == nullptr)
        firstNodeOfSecondaryTree->balanceFactor = firstNodeOfSecondaryTree->rightChild->height + 1;
    else
        firstNodeOfSecondaryTree->balanceFactor = firstNodeOfSecondaryTree->rightChild->height - firstNodeOfSecondaryTree->leftChild->height;
}

void setBalanceSecondaryTreeAVL(SecondaryTree* firstNodeOfSecondaryTree, SecondaryTree* const constantFirstNodeOfSecondaryTree, PrimaryTree* topTree){//en sonda performans analizi yaparsın, sonradan const yaptım

    if(firstNodeOfSecondaryTree->leftChild != nullptr)
        setBalanceSecondaryTreeAVL(firstNodeOfSecondaryTree->leftChild, constantFirstNodeOfSecondaryTree, topTree);
    if(firstNodeOfSecondaryTree->rightChild != nullptr)
        setBalanceSecondaryTreeAVL(firstNodeOfSecondaryTree->rightChild, constantFirstNodeOfSecondaryTree, topTree);


    SecondaryTree* ancestor = constantFirstNodeOfSecondaryTree;
    if(firstNodeOfSecondaryTree != constantFirstNodeOfSecondaryTree) {
        while (ancestor->leftChild != firstNodeOfSecondaryTree && ancestor->rightChild != firstNodeOfSecondaryTree) {
            if (firstNodeOfSecondaryTree->productName.compare(ancestor->productName) < 0)
                ancestor = ancestor->leftChild;
            else
                ancestor = ancestor->rightChild;
        }
    }
    else
        ancestor = nullptr;


    if(firstNodeOfSecondaryTree->balanceFactor == 2){
        if(firstNodeOfSecondaryTree->rightChild->rightChild == nullptr) {
            firstNodeOfSecondaryTree->rightChild->leftChild->rightChild = firstNodeOfSecondaryTree->rightChild;
            firstNodeOfSecondaryTree->rightChild->leftChild->leftChild = firstNodeOfSecondaryTree;
            if(ancestor != nullptr) {
                if (firstNodeOfSecondaryTree->productName.compare(ancestor->productName) < 0)
                    ancestor->leftChild = firstNodeOfSecondaryTree->rightChild->leftChild;
                else
                    ancestor->rightChild = firstNodeOfSecondaryTree->rightChild->leftChild;
                firstNodeOfSecondaryTree->rightChild->leftChild = nullptr;
                firstNodeOfSecondaryTree->rightChild = nullptr;
            }
            else{
                SecondaryTree* newTop = firstNodeOfSecondaryTree->rightChild->leftChild;
                firstNodeOfSecondaryTree->rightChild->leftChild = nullptr;
                firstNodeOfSecondaryTree->rightChild = nullptr;
                topTree->firstNodeOfSecondaryTree = newTop;
            }
        }
        else{
            SecondaryTree* temp = firstNodeOfSecondaryTree->rightChild->leftChild;
            firstNodeOfSecondaryTree->rightChild->leftChild = firstNodeOfSecondaryTree;
            if(ancestor != nullptr) {
                if (firstNodeOfSecondaryTree->productName.compare(ancestor->productName) < 0)
                    ancestor->leftChild = firstNodeOfSecondaryTree->rightChild;
                else
                    ancestor->rightChild = firstNodeOfSecondaryTree->rightChild;
                firstNodeOfSecondaryTree->rightChild = temp;
            }
            else{
                topTree->firstNodeOfSecondaryTree = firstNodeOfSecondaryTree->rightChild;
                firstNodeOfSecondaryTree->rightChild = temp;

            }
        }
    }

    else if(firstNodeOfSecondaryTree->balanceFactor == -2){
        if(firstNodeOfSecondaryTree->leftChild->leftChild == nullptr) {
            firstNodeOfSecondaryTree->leftChild->rightChild->leftChild = firstNodeOfSecondaryTree->leftChild;
            firstNodeOfSecondaryTree->leftChild->rightChild->rightChild = firstNodeOfSecondaryTree;
            if(ancestor != nullptr) {
                if (firstNodeOfSecondaryTree->productName.compare(ancestor->productName) < 0)
                    ancestor->leftChild = firstNodeOfSecondaryTree->leftChild->rightChild;
                else
                    ancestor->rightChild = firstNodeOfSecondaryTree->leftChild->rightChild;
                firstNodeOfSecondaryTree->leftChild->rightChild = nullptr;
                firstNodeOfSecondaryTree->leftChild = nullptr;
            }
            else{
                SecondaryTree* newTop = firstNodeOfSecondaryTree->leftChild->rightChild;
                firstNodeOfSecondaryTree->leftChild->rightChild = nullptr;
                firstNodeOfSecondaryTree->leftChild = nullptr;
                topTree->firstNodeOfSecondaryTree = newTop;
            }
        }
        else{
            SecondaryTree* temp = firstNodeOfSecondaryTree->leftChild->rightChild;
            firstNodeOfSecondaryTree->leftChild->rightChild = firstNodeOfSecondaryTree;
            if(ancestor != nullptr) {
                if (firstNodeOfSecondaryTree->productName.compare(ancestor->productName) < 0)
                    ancestor->leftChild = firstNodeOfSecondaryTree->leftChild;
                else
                    ancestor->rightChild = firstNodeOfSecondaryTree->leftChild;
                firstNodeOfSecondaryTree->leftChild = temp;
            }
            else{
                topTree->firstNodeOfSecondaryTree = firstNodeOfSecondaryTree->leftChild;
                firstNodeOfSecondaryTree->leftChild = temp;
            }
        }
    }
    findHeightsOfAVL(topTree->firstNodeOfSecondaryTree);
    calculateBalanceFactors(topTree->firstNodeOfSecondaryTree);
}

void insertAVL(string category, string name, int price, PrimaryTree* firstNodeOfPrimaryTree){
    if(firstNodeOfPrimaryTree->isExist == false){
        firstNodeOfPrimaryTree->isExist = true;
        firstNodeOfPrimaryTree->categoryName = category;
        firstNodeOfPrimaryTree->firstNodeOfSecondaryTree = new SecondaryTree;
        firstNodeOfPrimaryTree->firstNodeOfSecondaryTree->categoryName = category;
        firstNodeOfPrimaryTree->firstNodeOfSecondaryTree->productName = name;
        firstNodeOfPrimaryTree->firstNodeOfSecondaryTree->data = price;
    }
    else{
        PrimaryTree* temp = firstNodeOfPrimaryTree;
        while(temp->categoryName != category) {
            if (category.compare(temp->categoryName) < 0) {
                if(temp->leftChild != nullptr)
                    temp = temp->leftChild;
                else
                    break;
            }
            else {
                if(temp->rightChild != nullptr)
                    temp = temp->rightChild;
                else
                    break;
            }
        }

        if(temp->categoryName == category){

            SecondaryTree* newNode = temp->firstNodeOfSecondaryTree;
            while(true) {
                if (name.compare(newNode->productName) < 0) {
                    if(newNode->leftChild != nullptr)
                        newNode = newNode->leftChild;
                    else{
                        newNode->leftChild = new SecondaryTree;
                        newNode->leftChild->categoryName = category;
                        newNode->leftChild->productName = name;
                        newNode->leftChild->data = price;
                        break;
                    }
                }
                else {
                    if(newNode->rightChild != nullptr)
                        newNode = newNode->rightChild;
                    else{
                        newNode->rightChild = new SecondaryTree;
                        newNode->rightChild->categoryName = category;
                        newNode->rightChild->productName = name;
                        newNode->rightChild->data = price;
                        break;
                    }
                }
            }

            findHeightsOfAVL(temp->firstNodeOfSecondaryTree);
            calculateBalanceFactors(temp->firstNodeOfSecondaryTree);
            setBalanceSecondaryTreeAVL(temp->firstNodeOfSecondaryTree, temp->firstNodeOfSecondaryTree, temp);
        }
        else{
            if(category.compare(temp->categoryName) < 0){
                temp->leftChild = new PrimaryTree;
                temp->leftChild->categoryName = category;
                temp->leftChild->firstNodeOfSecondaryTree = new SecondaryTree;
                temp->leftChild->firstNodeOfSecondaryTree->categoryName = category;
                temp->leftChild->firstNodeOfSecondaryTree->productName = name;
                temp->leftChild->firstNodeOfSecondaryTree->data = price;
            }
            else{
                temp->rightChild = new PrimaryTree;
                temp->rightChild->categoryName = category;
                temp->rightChild->firstNodeOfSecondaryTree = new SecondaryTree;
                temp->rightChild->firstNodeOfSecondaryTree->categoryName = category;
                temp->rightChild->firstNodeOfSecondaryTree->productName = name;
                temp->rightChild->firstNodeOfSecondaryTree->data = price;
            }
        }
    }
}

void removeAVL(string category, string name, PrimaryTree* firstNodeOfPrimaryTree){
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    while(primaryNode->categoryName != category){
        if (category.compare(primaryNode->categoryName) < 0) {
            primaryNode = primaryNode->leftChild;
        }
        else {
            primaryNode = primaryNode->rightChild;
        }
        if(primaryNode == nullptr){
            return;
        }
    }

    SecondaryTree* willBeDeleted = primaryNode->firstNodeOfSecondaryTree;
    while(willBeDeleted->productName != name){
        if (name.compare(willBeDeleted->productName) < 0) {
            willBeDeleted = willBeDeleted->leftChild;
        }
        else {
            willBeDeleted = willBeDeleted->rightChild;
        }
        if(willBeDeleted == nullptr)
            return;
    }

    SecondaryTree* father = nullptr;
    if(willBeDeleted != primaryNode->firstNodeOfSecondaryTree) {
        father = primaryNode->firstNodeOfSecondaryTree;
        while (father->leftChild != willBeDeleted && father->rightChild != willBeDeleted) {
            if (name.compare(father->productName) < 0) {
                father = father->leftChild;
            } else {
                father = father->rightChild;
            }
        }
    }

    if(willBeDeleted->leftChild == nullptr && willBeDeleted->rightChild == nullptr){
        if(father != nullptr) {
            if (name.compare(father->productName) < 0) {
                father->leftChild = nullptr;
                delete willBeDeleted;
            } else {
                father->rightChild = nullptr;
                delete willBeDeleted;
            }
            findHeightsOfAVL(primaryNode->firstNodeOfSecondaryTree);
            calculateBalanceFactors(primaryNode->firstNodeOfSecondaryTree);
            setBalanceSecondaryTreeAVL(primaryNode->firstNodeOfSecondaryTree, primaryNode->firstNodeOfSecondaryTree, primaryNode);
        }
        else {
            primaryNode->firstNodeOfSecondaryTree = nullptr;
            delete willBeDeleted;
        }

    }

    else if(willBeDeleted->leftChild == nullptr && willBeDeleted->rightChild != nullptr){
        if(father != nullptr) {
            if (name.compare(father->productName) < 0) {
                father->leftChild = willBeDeleted->rightChild;
                delete willBeDeleted;
            }
            else {
                father->rightChild = willBeDeleted->rightChild;
                delete willBeDeleted;
            }
        }
        else{
            primaryNode->firstNodeOfSecondaryTree = willBeDeleted->rightChild;
            delete willBeDeleted;
        }
        findHeightsOfAVL(primaryNode->firstNodeOfSecondaryTree);
        calculateBalanceFactors(primaryNode->firstNodeOfSecondaryTree);
        setBalanceSecondaryTreeAVL(primaryNode->firstNodeOfSecondaryTree, primaryNode->firstNodeOfSecondaryTree, primaryNode);
    }

    else if(willBeDeleted->rightChild == nullptr && willBeDeleted->leftChild != nullptr){
        if(father != nullptr) {
            if (name.compare(father->productName) < 0) {
                father->leftChild = willBeDeleted->leftChild;
                delete willBeDeleted;
            }
            else {
                father->rightChild = willBeDeleted->leftChild;
                delete willBeDeleted;
            }
        }
        else{
            primaryNode->firstNodeOfSecondaryTree = willBeDeleted->leftChild;
            delete willBeDeleted;
        }
        findHeightsOfAVL(primaryNode->firstNodeOfSecondaryTree);
        calculateBalanceFactors(primaryNode->firstNodeOfSecondaryTree);
        setBalanceSecondaryTreeAVL(primaryNode->firstNodeOfSecondaryTree, primaryNode->firstNodeOfSecondaryTree, primaryNode);
    }

    else if(willBeDeleted->rightChild != nullptr && willBeDeleted->leftChild != nullptr){
        SecondaryTree* maxOfLeftSide = willBeDeleted->leftChild;
        while(maxOfLeftSide->rightChild != nullptr)
            maxOfLeftSide = maxOfLeftSide->rightChild;
        if(father != nullptr) {
            if (name.compare(father->productName) < 0) {
                *(father->leftChild) = *(maxOfLeftSide);
                removeAVL(maxOfLeftSide->categoryName, maxOfLeftSide->productName, firstNodeOfPrimaryTree);
            }
            else {
                *(father->rightChild) = *(maxOfLeftSide);
                removeAVL(maxOfLeftSide->categoryName, maxOfLeftSide->productName, firstNodeOfPrimaryTree);
            }
        }
        else{
            *(primaryNode->firstNodeOfSecondaryTree) = *(maxOfLeftSide);
            removeAVL(maxOfLeftSide->categoryName, maxOfLeftSide->productName, firstNodeOfPrimaryTree);
        }
        findHeightsOfAVL(primaryNode->firstNodeOfSecondaryTree);
        calculateBalanceFactors(primaryNode->firstNodeOfSecondaryTree);
        setBalanceSecondaryTreeAVL(primaryNode->firstNodeOfSecondaryTree, primaryNode->firstNodeOfSecondaryTree, primaryNode);
    }
}


void printLevel(SecondaryTree* firstNodeOfSecondaryTree, int initialLevel){
    if(firstNodeOfSecondaryTree == nullptr)
        return;
    else if(initialLevel == 0)
        writeToOutputFile << firstNodeOfSecondaryTree->productName << ":" << firstNodeOfSecondaryTree->data << " ";
    else {
        printLevel(firstNodeOfSecondaryTree->leftChild, initialLevel - 1);
        printLevel(firstNodeOfSecondaryTree->rightChild, initialLevel - 1);
    }
}


void printAllItemsInCategory(string category, PrimaryTree* firstNodeOfPrimaryTree){
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    if(primaryNode == nullptr){
        writeToOutputFile << "command:printAllItemsInCategory " << " " << category << endl << "{" << endl << "{}" << endl << "}" << endl;
        return;
    }
    while(primaryNode->categoryName != category){
        if (category.compare(primaryNode->categoryName) < 0)
            primaryNode = primaryNode->leftChild;
        else
            primaryNode = primaryNode->rightChild;
        if(primaryNode == nullptr){
            writeToOutputFile << "command:printAllItemsInCategory " << " " << category << endl << "{" << endl << "{}" << endl << "}" << endl;
            return;
        }
    }
    SecondaryTree* firstNodeOfSecondaryTree = primaryNode->firstNodeOfSecondaryTree;
    if(firstNodeOfSecondaryTree == nullptr) {
        writeToOutputFile << "command:printAllItemsInCategory " << " " << category << endl << "{" << endl << category << ":{}" << endl << "}" << endl;
        return;
    }
    int height = firstNodeOfSecondaryTree->height;
    writeToOutputFile << "command:printAllItemsInCategory " << " " << category << endl << "{" << endl << category << ":" << endl;
    for(int i = 0; i <= height; i++) {
        writeToOutputFile<<"\t";
        printLevel(firstNodeOfSecondaryTree, i);
        writeToOutputFile<<endl;
    }
    writeToOutputFile<<"}"<<endl;
}


void printPrimaryTreesLevel(PrimaryTree* firstNodeOfPrimaryTree, int initialLevel){
    if(firstNodeOfPrimaryTree == nullptr){
        return;
    }
    else if(initialLevel == 0) {
        SecondaryTree* firstNodeOfSecondaryTree = firstNodeOfPrimaryTree->firstNodeOfSecondaryTree;
        if(firstNodeOfSecondaryTree == nullptr) {
            writeToOutputFile << firstNodeOfPrimaryTree->categoryName << ":{}" << endl;
            return;
        }
        int height = firstNodeOfSecondaryTree->height;
        writeToOutputFile << firstNodeOfPrimaryTree->categoryName << ":" << endl;
        for(int i = 0; i <= height; i++) {
            writeToOutputFile<<"\t";
            printLevel(firstNodeOfSecondaryTree, i);
            writeToOutputFile<<endl;
        }
    }
    else {
        printPrimaryTreesLevel(firstNodeOfPrimaryTree->leftChild, initialLevel - 1);
        printPrimaryTreesLevel(firstNodeOfPrimaryTree->rightChild, initialLevel - 1);
    }
}


void printAllItems(PrimaryTree* firstNodeOfPrimaryTree, int height){
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    if(primaryNode == nullptr){
        writeToOutputFile<<"command:printAllItems"<<endl<<"{"<<endl<<"{}"<<endl<<"}";
        return;
    }
    writeToOutputFile<<"command:printAllItems"<<endl<<"{"<<endl;
    for(int i = 0; i <= height; i++) {
        printPrimaryTreesLevel(firstNodeOfPrimaryTree, i);
    }
    writeToOutputFile<<"}"<<endl;
}


void printItem(string category, string name, PrimaryTree* firstNodeOfPrimaryTree){
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    if(primaryNode == nullptr) {
        writeToOutputFile << "command:printItem\t" << category << "\t" << name << endl << "{}" << endl;
        return;
    }
    while(primaryNode->categoryName != category){
        if (category.compare(primaryNode->categoryName) < 0)
            primaryNode = primaryNode->leftChild;
        else
            primaryNode = primaryNode->rightChild;
        if(primaryNode == nullptr) {
            writeToOutputFile << "command:printItem\t" << category << "\t" << name << endl << "{}" << endl;
            return;
        }
    }
    SecondaryTree* secondaryNode = primaryNode->firstNodeOfSecondaryTree;
    if(secondaryNode == nullptr){
        writeToOutputFile << "command:printItem\t" << category << "\t" << name << endl << "{}" << endl;
        return;
    }
    else{
        while (secondaryNode->productName != name) {
            if (name.compare(secondaryNode->productName) < 0)
                secondaryNode = secondaryNode->leftChild;
            else
                secondaryNode = secondaryNode->rightChild;
            if(secondaryNode == nullptr){
                writeToOutputFile << "command:printItem\t" << category << "\t" << name << endl << "{}" << endl;
                return;
            }
        }
    }
    writeToOutputFile << "command:printItem\t" << category << "\t" << name << endl << "{" << endl;
    writeToOutputFile << category << ":" << endl << "\t" << name << ":" << secondaryNode->data << endl << "}" << endl;
}


void findProduct(string category, string name, PrimaryTree* firstNodeOfPrimaryTree){
    writeToOutputFile<<"command:find\t"<<category<<"\t"<<name<<endl;
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    if(primaryNode == nullptr) {
        writeToOutputFile << "{}" << endl;
        return;
    }
    while(primaryNode->categoryName != category){
        if (category.compare(primaryNode->categoryName) < 0)
            primaryNode = primaryNode->leftChild;
        else
            primaryNode = primaryNode->rightChild;
        if(primaryNode == nullptr) {
            writeToOutputFile << "{}" << endl;
            return;
        }
    }
    SecondaryTree* secondaryNode = primaryNode->firstNodeOfSecondaryTree;
    while(secondaryNode->productName != name){
        if (name.compare(secondaryNode->productName) < 0)
            secondaryNode = secondaryNode->leftChild;
        else
            secondaryNode = secondaryNode->rightChild;
        if(secondaryNode == nullptr) {
            writeToOutputFile << "{}" << endl;
            return;
        }
    }
    writeToOutputFile<< "{" << endl <<category<<":"<<endl<<"\t"<<name<<":"<<secondaryNode->data<<endl<<"}"<<endl;
}


void updateData(string category, string name, int newData, PrimaryTree* firstNodeOfPrimaryTree){
    PrimaryTree* primaryNode = firstNodeOfPrimaryTree;
    if(primaryNode == nullptr)
        return;
    while(primaryNode->categoryName != category){
        if (category.compare(primaryNode->categoryName) < 0)
            primaryNode = primaryNode->leftChild;
        else
            primaryNode = primaryNode->rightChild;
        if(primaryNode == nullptr)
            return;
    }
    SecondaryTree* secondaryNode = primaryNode->firstNodeOfSecondaryTree;
    if(secondaryNode == nullptr)
        return;
    while(secondaryNode->productName != name) {
        if (name.compare(secondaryNode->productName) < 0)
            secondaryNode = secondaryNode->leftChild;
        else
            secondaryNode = secondaryNode->rightChild;
        if(secondaryNode == nullptr)
            return;
    }
    secondaryNode->data = newData;
}



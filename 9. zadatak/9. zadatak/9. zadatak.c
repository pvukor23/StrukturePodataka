#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode* position;
typedef struct TreeNode {
    int el;
    position leftChild, rightChild;
} treeNode;

position insert(position root, int newEl);
position createNode(int newEl);
int replace(position root);
int levelorderPrint(position root, FILE* RTFile);  // Modified to print to file with levels
int random();
int randTreeFileLevelOrderPrint(position root, FILE* RTFile);  // Updated to print level order with spaces and newlines

int main() {
    position root = NULL, rootRand = NULL;
    srand(time(0));
    int i = 0;
    FILE* RTFile = NULL;

    //a)
    root = insert(root, 2);
    insert(root, 5);
    insert(root, 7);
    insert(root, 8);
    insert(root, 11);
    insert(root, 1);
    insert(root, 4);
    insert(root, 2);
    insert(root, 3);
    insert(root, 7);

    // Print level order to terminal
    printf("Level order print (a):\n");
    levelorderPrint(root, NULL);  // Print to terminal
    printf("\n");

    //b)
    replace(root);

    // Print level order to terminal after replacement
    printf("Level order print after replacement (b):\n");
    levelorderPrint(root, NULL);  // Print to terminal    
    printf("\n");

    //c) Generate random values and insert into the tree
    rootRand = insert(rootRand, random());
    for (i = 0; i < 9; i++)
        insert(rootRand, random());

    // Open "tree.txt" to write the random level-order traversal
    RTFile = fopen("tree.txt", "w");

    // Write the random level-order print to the file
    fprintf(RTFile, "Random level order print (c):\n");
    randTreeFileLevelOrderPrint(rootRand, RTFile);  // Print random level order to the file
    fprintf(RTFile, "\n");

    replace(rootRand);

    // Write the level order of the tree after replacement into the file
    fprintf(RTFile, "Level order print after replacement (c):\n");
    levelorderPrint(rootRand, RTFile);  // Write level order to the file
    fprintf(RTFile, "\n");

    // Close the file after writing
    fclose(RTFile);

    return EXIT_SUCCESS;
}

// Function to print tree in level order with spaces and new lines
// Modified to support file output and level-by-level printing
int levelorderPrint(position root, FILE* RTFile) {
    position current = NULL, queue[100] = { NULL };
    int front = 0, rear = 0, levelSize = 1;  // Keep track of the level size

    if (root == NULL)
        return EXIT_SUCCESS;

    queue[rear] = root;

    while (front <= rear) {
        current = queue[front];
        levelSize--;

        if (RTFile) {
            fprintf(RTFile, "%d ", current->el);  // Write to file if RTFile is not NULL
        }
        else {
            printf("%d ", current->el);  // Otherwise, print to terminal
        }

        if (current->leftChild != NULL)
            queue[++rear] = current->leftChild;

        if (current->rightChild != NULL)
            queue[++rear] = current->rightChild;

        front++;

        // If we reach the end of the current level, print a newline
        if (levelSize == 0) {
            if (RTFile) {
                fprintf(RTFile, "\n");
            }
            else {
                printf("\n");
            }
            levelSize = rear - front + 1;  // Update level size for the next level
        }
    }

    return EXIT_SUCCESS;
}

// Insert function to insert elements into the binary tree
position insert(position root, int newEl) {

    if (root == NULL)
        return createNode(newEl);

    if (newEl < root->el)
        root->leftChild = insert(root->leftChild, newEl);
    else if (newEl >= root->el)
        root->rightChild = insert(root->rightChild, newEl);

    return root;
}

// Function to create a new node
position createNode(int newEl) {
    position newNode = NULL;

    newNode = malloc(sizeof(treeNode));

    if (!newNode) {
        printf("\nNeuspjela alokacija memorije za newNode!\n");
        return NULL;
    }

    newNode->el = newEl;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    return newNode;
}

// Function to replace the node values with the sum of their children
int replace(position root) {
    int leftEl = 0, rightEl = 0, ogEl = 0;

    if (root == NULL)
        return 0;

    ogEl = root->el;

    leftEl = replace(root->leftChild);
    rightEl = replace(root->rightChild);

    root->el = leftEl + rightEl;

    return ogEl + root->el;
}

// Random function to generate numbers
int random() {
    return (rand() % (90 - 10 + 1)) + 10;
}

// Function to print the random level order into the file with spaces and newlines
int randTreeFileLevelOrderPrint(position root, FILE* RTFile) {
    position current = NULL, queue[100] = { NULL };
    int front = 0, rear = 0, levelSize = 1;  // Keep track of the level size

    if (root == NULL)
        return EXIT_SUCCESS;

    queue[rear] = root;

    while (front <= rear) {
        current = queue[front];
        levelSize--;

        // Print random values instead of inorder traversal
        fprintf(RTFile, "%d ", random());  // Write random value to file

        if (current->leftChild != NULL)
            queue[++rear] = current->leftChild;

        if (current->rightChild != NULL)
            queue[++rear] = current->rightChild;

        front++;

        // If we reach the end of the current level, print a newline
        if (levelSize == 0) {
            fprintf(RTFile, "\n");  // Newline for each level in the tree
            levelSize = rear - front + 1;  // Update level size for the next level
        }
    }

    return EXIT_SUCCESS;
}

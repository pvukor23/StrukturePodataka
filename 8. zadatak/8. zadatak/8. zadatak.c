#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>

struct _tree;
typedef struct _tree* tp;

typedef struct _tree {
    int value;
    tp right;
    tp left;
} tree;

// Funkcije za rad sa stablom
tree* Insert(tree* root, int value);
tree* Create(int value);
void Preorder(tree* root);
void Inorder(tree* root);
void Postorder(tree* root);
void Level(tree* root);
int Height(tree* root);
void Printcurrentlevel(tree* root, int level);
int Search(tree* root, int value);
tree* Delete(tree* root, int value);
tree* Min(tree* root);

int main() {
    tree* root = NULL;

    int numbers[] = { 5, 3, 2, 4, 7, 6, 8 };
    int n = sizeof(numbers) / sizeof(numbers[0]);

    // Umetanje elemenata u stablo
    for (int i = 0; i < n; i++) {
        root = Insert(root, numbers[i]);
    }

    int choice, value;

    do {
        printf("Odaberi opciju: \n");
        printf("\n1. Dodaj element\n");
        printf("2. Ispis (preorder)\n");
        printf("3. Ispis (inorder)\n");
        printf("4. Ispis (postorder)\n");
        printf("5. Ispis (level order)\n");
        printf("6. Pronadi element\n");
        printf("7. Izbrisi element\n");
        printf("8. Izlaz\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = Insert(root, value);
            break;
        case 2:
            Preorder(root);
            printf("\n");
            break;
        case 3:
            Inorder(root);
            printf("\n");
            break;
        case 4:
            Postorder(root);
            printf("\n");
            break;
        case 5:
            Level(root);
            printf("\n");
            break;
        case 6:
            printf("Unesi vrijednost za pretragu: ");
            scanf("%d", &value);

            if (Search(root, value) == 1)
                printf("Element pronaden.\n");
            else {
                printf("Element ne postoji.\n");
            }
            break;
        case 7:
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            root = Delete(root, value);
            break;
        case 8:
            printf("Izlaz.\n");
            break;
        default:
            printf("Krivi unos.\n ");
        }

    } while (choice != 8);

    return 0;
}

// Kreira novi čvor sa zadanim vrijednostima
tree* Create(int value) {
    tree* new = (tree*)malloc(sizeof(tree));
    new->value = value;
    new->left = new->right = NULL;
    return new;
}

// Umetanje novog elementa u stablo
tree* Insert(tree* root, int value) {
    if (root == NULL)
        return Create(value);

    if (value < root->value)
        root->left = Insert(root->left, value);
    else if (value > root->value)
        root->right = Insert(root->right, value);

    return root;
}

// Preorder ispis (Root, Left, Right)
void Preorder(tree* root) {
    if (root != NULL) {
        printf("%d ", root->value); // Ispisuje root
        Preorder(root->left);        // Ispisuje lijevo podstablo
        Preorder(root->right);       // Ispisuje desno podstablo
    }
}

// Inorder ispis (Left, Root, Right)
void Inorder(tree* root) {
    if (root != NULL) {
        Inorder(root->left);        // Ispisuje lijevo podstablo
        printf("%d ", root->value); // Ispisuje root
        Inorder(root->right);       // Ispisuje desno podstablo
    }
}

// Postorder ispis (Left, Right, Root)
void Postorder(tree* root) {
    if (root != NULL) {
        Postorder(root->left);      // Ispisuje lijevo podstablo
        Postorder(root->right);     // Ispisuje desno podstablo
        printf("%d ", root->value); // Ispisuje root
    }
}

// Ispis stabla po razini
void Level(tree* root) {
    int h = Height(root);
    for (int i = 0; i <= h; i++)
        Printcurrentlevel(root, i);
}

// Funkcija za računanje visine stabla
int Height(tree* root) {
    if (root == NULL)
        return 0;

    int LH = Height(root->left);
    int RH = Height(root->right);
    int max = (RH > LH) ? RH : LH;

    return max + 1;
}

// Ispis svih čvorova na određenoj razini
void Printcurrentlevel(tree* root, int level) {
    if (root == NULL)
        return;

    if (level == 1)
        printf("%d ", root->value); // Ispisuje vrijednost na trenutnoj razini
    else if (level > 1) {
        Printcurrentlevel(root->left, level - 1);  // Ispisuje lijevo podstablo
        Printcurrentlevel(root->right, level - 1); // Ispisuje desno podstablo
    }
}

// Pretraga elementa u stablu
int Search(tree* root, int value) {
    if (root == NULL)
        return 0;
    if (root->value == value)
        return 1;
    if (value < root->value)
        return Search(root->left, value);
    else
        return Search(root->right, value);
}

// Pronađe minimum u desnom podstablu
tree* Min(tree* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Brisanje elementa iz stabla
tree* Delete(tree* root, int value) {
    if (root == NULL)
        return root;

    if (value < root->value)
        root->left = Delete(root->left, value);
    else if (value > root->value)
        root->right = Delete(root->right, value);
    else {
        // Čvor sa samo desnim ili lijevim podstablom
        if (root->right == NULL && root->left == NULL) {
            free(root);
            root = NULL;
        }
        else if (root->left == NULL) {
            tree* temp = root;
            root = root->right;
            free(temp);
        }
        else if (root->right == NULL) {
            tree* temp = root;
            root = root->left;
            free(temp);
        }
        else {
            tree* temp = Min(root->right);
            root->value = temp->value;
            root->right = Delete(root->right, temp->value);
        }
    }
    return root;
}

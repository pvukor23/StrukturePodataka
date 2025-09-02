/*Napisati program koji čita datoteku koja sadrži podatke o kategorijama proizvoda „kategorije.txt“ 
te kreirati vezanu listu kategorija sortiranu po abecedi. Uz kategoriju proizvoda u datoteci se nalaze i 
podaci o minimalnoj i maksimalnoj cijeni proizvoda u toj kategoriji. Nakon toga iz datoteke „proizvodi.txt“, 
treba pročitati podatke o proizvodima i kategorijama kojim pripadaju.
Svaki proizvod treba smjestiti u dodatnu vezanu listu za pojedinu kategoriju, tj.
svaka kategorija sadrži dodatnu vezanu listu proizvoda koji spadaju u tu kategoriju.
Za svaki proizvod iz kategorije treba izgenerirati cijenu(slučajan broj iz min i maks opsega kategorije) 
i ne smije biti ponavljanja brojeva unutar iste kategorije. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MAX_NAME 32

struct _product;
typedef struct _product* ProductP;
typedef struct _product {
    char name[MAX_NAME];
    int price;
    ProductP next;
} Product;

struct _category;
typedef struct _category* CategoryP;
typedef struct _category {
    char name[MAX_NAME];    
    int minPrice;             
    int maxPrice;             
    ProductP products;        
    CategoryP next;   
} Category;


int insertSroted(CategoryP head, CategoryP newCategory);
CategoryP createCategory(char* name, int minPrice, int maxPrice);
int readFromFileCategory(char* filename, CategoryP head);
int printListCategory(CategoryP first);
int addProductToCategory(CategoryP category, ProductP product);
ProductP createProduct(char* name, int price);
int generateRandomPrice(CategoryP category);
CategoryP findCategory(CategoryP head, char* nameCategory);
int readFromFileProduct(char* filename, CategoryP head);
int printAllProducts(CategoryP first);
int deleteProducts(ProductP first);
int deleteListCategory(CategoryP head);

int main()
{
    Category head = { .name = { 0 }, .minPrice = 0, .maxPrice = 0, .products = NULL, .next = NULL };

    srand(time(NULL));

    if (readFromFileCategory("kategorije.txt", &head) != EXIT_SUCCESS)
    {
        perror("File could not be opened!");
        return FILE_OPEN_ERROR;
    }

    // Ispis liste kategorija
    printf("\nLista kategorija:\n");
    printListCategory(head.next);

    if (readFromFileProduct("proizvodi.txt", &head) != EXIT_SUCCESS)
    {
        perror("File could not be opened!");
        return FILE_OPEN_ERROR;
    }
    // Ispis liste kategorija
    printf("\nLista proizvoda:\n");
    printAllProducts(head.next);

    // Oslobađanje memorije
    printf("\nOslobadanje memorije...\n");
    deleteListCategory(&head);

    return EXIT_SUCCESS;
}

int insertSroted(CategoryP head, CategoryP newCategory)
{
    CategoryP previous = head;
    CategoryP current = head->next;

    while (current != NULL && strcmp(current->name, newCategory->name) < 0)
    {
        previous = current;
        current = current->next;
    }

    newCategory->next = current;
    previous->next = newCategory;

    return EXIT_SUCCESS;
}

CategoryP createCategory(char* name, int minPrice, int maxPrice)
{
    CategoryP newCategory = NULL;

    newCategory = (CategoryP)malloc(sizeof(Category));
    if (newCategory == NULL)    // if (!newCategory)      
    {
        perror("Memory allocation failed!");
        return NULL;
    }

    strcpy(newCategory->name, name);
    newCategory->minPrice = minPrice;
    newCategory->maxPrice = maxPrice;
    newCategory->products = NULL;
    newCategory->next = NULL;

    return newCategory;
}

int readFromFileCategory(char* filename, CategoryP head)
{
    FILE* filePointer = NULL;
    char name[MAX_NAME] = { 0 };
    int minPrice = 0;
    int maxPrice = 0;
    CategoryP newCategory = NULL;

    filePointer = fopen(filename, "r");
    if (filePointer == NULL)   // if (!filePointer) 
    {
        perror("File could not be opened!");
        return FILE_OPEN_ERROR;
    }

    while (fscanf(filePointer, "%s %d %d", name, &minPrice, &maxPrice) == 3)
    {
        newCategory = createCategory(name, minPrice, maxPrice);
        if (newCategory == NULL)    // if (!newCategory)
        {
            perror("Memory allocation failed!");
            fclose(filePointer);
            return MALLOC_ERROR;
        }
        insertSroted(head, newCategory);
    }

    fclose(filePointer);


    return EXIT_SUCCESS;
}

int printListCategory(CategoryP first)
{
    CategoryP current = first;

    if (current == NULL)
    {
        printf("Empty list!\n");
        return  EMPTY_LIST;
    }

    while (current != NULL)
    {
        printf("Kategorija: %-31s  Min_cijena: %-10d  Max_cijena: %-10d\n", current->name, current->minPrice, current->maxPrice);
        current = current->next;
    }

    return EXIT_SUCCESS;
}

int addProductToCategory(CategoryP category, ProductP product)
{
    ProductP currentProduct = NULL;
    
    if (category->products == NULL) 
    {
        category->products = product;
    }
    else 
    {
        currentProduct = category->products;
        while (currentProduct->next != NULL)
            currentProduct = currentProduct->next;
        
        currentProduct->next = product;
    }

    return EXIT_SUCCESS;
}

ProductP createProduct(char* name, int price)
{
    ProductP newProduct = NULL;

    newProduct = (ProductP)malloc(sizeof(Product));
    if (newProduct == NULL)    // if (!newProduct)      
    {
        perror("Memory allocation failed!");
        return NULL;
    }

    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->next = NULL;

    return newProduct;
}

int generateRandomPrice(CategoryP category)
{
    int randomPrice = 0;
    int unique;
    ProductP product = NULL;

    do {
        unique = 1;
        randomPrice = category->minPrice + rand() % (category->maxPrice - category->minPrice + 1);

        product = category->products;
        while (product != NULL)
        {
            if (product->price == randomPrice)
            {
                unique = 0;
                break;
            }
            product = product->next;
        }

    } while (unique == 0);
    

    return randomPrice;
}

//CategoryP findCategory(CategoryP head, char* nameCategory)
//{
//    CategoryP category = head;
//
//    while (category != NULL && strcmp(category->name, nameCategory) != 0)
//        category = category->next;
//
//    if (category != NULL)
//        return category;
//    else
//        return NULL;
//}

CategoryP findCategory(CategoryP head, char* nameCategory)
{
    CategoryP category = head;

    while (category != NULL)
    {
        if (strcmp(category->name, nameCategory) == 0)
            return category;

        category = category->next;
    }
    
    return category;
}

int readFromFileProduct(char* filename, CategoryP head)
{
    FILE* filePointer = NULL;
    char nameCategory[MAX_NAME] = { 0 };
    char name[MAX_NAME] = { 0 };
    int price = 0;
    CategoryP category = NULL;
    ProductP newProduct = NULL;

    filePointer = fopen(filename, "r");
    if (filePointer == NULL)   // if (!filePointer) 
    {
        perror("File could not be opened!");
        return FILE_OPEN_ERROR;
    }

    while (fscanf(filePointer, "%s %s", name, nameCategory) == 2)
    {
        category = findCategory(head, nameCategory);
        if (category == NULL)   // if (!category)
        {
            printf("Category does not exist!\n");
            continue;
        }
        else
        {
            price = generateRandomPrice(category);
            newProduct = createProduct(name, price);
            if (newProduct == NULL)    // if (!newProduct)
            {
                perror("Memory allocation failed!");
                fclose(filePointer);
                return MALLOC_ERROR;
            }
            addProductToCategory(category, newProduct);
        }
    }

    fclose(filePointer);


    return EXIT_SUCCESS;
}

int printAllProducts(CategoryP first)
{
    CategoryP currentCategory= first;
    ProductP currentProduct = NULL;

    if (currentCategory == NULL)
    {
        printf("Empty list of categories!\n");
        return  EMPTY_LIST;
    }

    while (currentCategory != NULL)
    {
        currentProduct = currentCategory->products;
        
        if (currentProduct== NULL)
        {
            printf("Category %s has no products.\n", currentCategory->name);
            // coninue; bi priskako cili ostatak petlje i currentCategory = currentCategory->next; pa bi stalno stao na jednoj kategoriji
        }
        else
        {
            while (currentProduct != NULL)
            {
                printf("Proizvod: %-31s Kategorije: %-31s Cijena: %-10d\n", currentProduct->name, currentCategory->name, currentProduct->price);
                currentProduct = currentProduct->next;
            }
        }
        currentCategory = currentCategory->next;
    }

    return EXIT_SUCCESS;
}

int deleteProducts(ProductP first)
{
    ProductP current = first;
    ProductP toDelete = NULL;  // ili temp svejedno

    while (current != NULL)
    {
        toDelete = current;
        current = current->next;
        free(toDelete);
    }
    return EXIT_SUCCESS;
}

int deleteListCategory(CategoryP head)
{
    CategoryP previous = head;
    CategoryP toDelete = NULL;  // ili temp svejedno

    while (previous->next != NULL)
    {
        toDelete = previous->next;
        previous->next = previous->next->next;    // previous->next = toDelete->next;
        deleteProducts(toDelete->products);
        free(toDelete);
    }

    return EXIT_SUCCESS;
}
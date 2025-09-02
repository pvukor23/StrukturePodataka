/*
	Napisati program koji iz datoteke "prva.txt" cita rijeci. Od tih rijeci se kreira vezana lista na
	nacin da se svaka nova rijec dodaje u vezanu listu, a ukoliko rijec vec postoji u vezanoj listi
	tada se samo uvecava brojac. Nakon toga iz datoteke "druga.txt" se kreira druga vezana lista
	na isti nacin.
	Za generiranje obje liste koristiti iste funkcije (poziva se ponovo za svaku lisu pojedinacno).

	Ispisati obje liste i nakon toga ispisati samo one elemente koji se ponavljaju u obje liste vise
	od 5 puta.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MAX_WORD 16

struct _word;
typedef struct _word* WordP;
typedef struct _word {
	char word[MAX_WORD];
	int count;
	WordP next;
} Word;

int addWordToList(WordP head1, WordP newWord);
WordP createWord(char* word, int count);
//WordP findWord(WordP first, char* word);
int readFromFile(char* filename, WordP head1);
int printList(WordP first);
int printCommonWords(WordP first1, WordP first2);
int deleteList(WordP head1);

int main()
{
	Word head1 = { .word = { 0 }, .count = 0, .next = NULL };
	Word head2 = { .word = { 0 }, .count = 0, .next = NULL };

	if (readFromFile("prva.txt", &head1) != EXIT_SUCCESS)
	{
		perror("File could not be opened!\n");
		return FILE_OPEN_ERROR;
	}

	if (readFromFile("druga.txt", &head2) != EXIT_SUCCESS)
	{
		perror("File could not be opened!\n");
		return FILE_OPEN_ERROR;
	}

	// Ispis prve liste 
	printf("Prva lista:\n");
	printList(head1.next);

	// Ispis druge liste 
	printf("\nDruga lista:\n");
	printList(head2.next);

	// Ispis zajednickih elemenata >5
	printf("\nZajednicke rijeci (>5 puta u obje liste):\n");
	printCommonWords(head1.next, head2.next);

	// Oslobađanje prve memorije
	printf("\nOslobadanje prve memorije...\n");
	deleteList(&head1);

	// Oslobađanje druge memorije
	printf("\nOslobadanje druge memorije...\n");
	deleteList(&head2);

	return EXIT_SUCCESS;
}

int addWordToList(WordP head1, WordP newWord)
{
	WordP previous = head1;
	WordP current = head1->next;

	while (current != NULL)
	{																					
		if (strcmp(current->word, newWord->word) == 0)									//
		{																				//
			current->count += newWord->count;											//
			free(newWord);  // ne treba umetati novi čvor jer je količina samo povećana	//
			return EXIT_SUCCESS;														//
		}																				//
		previous = current;
		current = current->next;
	}

	newWord->next = current;
	previous->next = newWord;

	return EXIT_SUCCESS;
}

WordP createWord(char* word, int count)
{
	WordP newWord = NULL;

	newWord = (WordP)malloc(sizeof(Word));
	if (newWord == NULL)		// if (!newWord)
	{
		perror("Memory allocation failed!\n");
		return NULL;
	}

	strcpy(newWord->word, word);
	newWord->count = count;
	newWord->next = NULL;

	return newWord;
}

int readFromFile(char* filename, WordP head1)
{
	FILE* filePointer = NULL;
	char word[MAX_WORD] = { 0 };
	int count = 1;
	WordP newWord = NULL;

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!\n");
		return FILE_OPEN_ERROR;
	}

	while (fscanf(filePointer, "%15s", word) == 1)
	{
		newWord = createWord(word, count);
		if (newWord == NULL)	// if (!newWord)
		{
			perror("Memory allocation failed!\n");
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		addWordToList(head1, newWord);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

//WordP findWord(WordP first, char* word)
//{
//	WordP current = first;
//
//	while (current != NULL)
//	{
//		if (strcmp(current->word, word) == 0)
//			return current;
//
//		current = current->next;
//	}
//
//	return current;
//}
//
// bez bezpotrebne dinamicke alokacije
//int readFromFile(char* filename, WordP head)
//{
//	FILE* filePointer = NULL;
//	char word[MAX_WORD] = { 0 };
//	int count = 1;
//	WordP newWord = NULL;
//
//	filePointer = fopen(filename, "r");
//	if (filePointer == NULL)	// if (!filePointer)
//	{
//		perror("File could not be opened!\n");
//		return FILE_OPEN_ERROR;
//	}
//
//	while (fscanf(filePointer, "%s", word) == 1)
//	{
//		newWord = findWord(head->next, word);
//		if (newWord == NULL)
//		{
//			newWord = createWord(word, count);
//			if (newWord == NULL)	// if (!newWord)
//			{
//				perror("Memory allocation failed!\n");
//				fclose(filePointer);
//				return MALLOC_ERROR;
//			}
//			addWordToList(head, newWord);
//		}
//		else
//			newWord->count++;
//	}
//
//	fclose(filePointer);
//
//	return EXIT_SUCCESS;
//}

int printList(WordP first)
{
	WordP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return  EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("Rijec: %-15s Broj ponavljanja: %d\n", current->word, current->count);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printCommonWords(WordP first1, WordP first2)
{
	WordP current1 = first1;
	
	while (current1 != NULL)
	{
		WordP current2 = first2;
		while (current2 != NULL)
		{
			if (strcmp(current1->word, current2->word) == 0 && current1->count > 5 && current2->count > 5)
			{
				printf("Rijec: %-15s Broj ponavljanja u prvoj listi: %-10d Broj ponavljanja u drugoj listi: %d\n", 
				current1->word, current1->count, current2->count);
			}
			current2 = current2->next;
		}
		current1 = current1->next;
	}

	return EXIT_SUCCESS;
}

int deleteList(WordP head1)
{
	WordP previous = head1;
	WordP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

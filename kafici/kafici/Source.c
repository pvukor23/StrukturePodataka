/*9.Napisati program koji iz datoteke čita podatke o ljudima i kafićima u 
kojima su ti ljudi bili. Izgraditi vezanu listu u kojoj su elementi složeni 
po nazivu kafića i ispisati je, te za traženi kafić ispisati koliko ga je ljudi 
posjetilo i koji su to ljudi*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MAX_NAME_LEN 32

struct _person;
typedef struct _person* PersonP;
typedef struct _person {
	char firstName[MAX_NAME_LEN];
	char lastName[MAX_NAME_LEN];
	PersonP next;
} Person;

struct _caffeBar;
typedef struct _caffeBar* CaffeBarP;
typedef struct _caffeBar {
	char caffeBarName[MAX_NAME_LEN];
	PersonP visitors;
	CaffeBarP next;
} CaffeBar;

int addPersonToCaffeBarList(CaffeBarP caffeBar, PersonP newPerson);
PersonP createPerson(char* firstName, char* lastName);
int insertSortedCaffeBar(CaffeBarP head, CaffeBarP newcaffeBar);
CaffeBarP createcaffeBar(char* caffeBarName);
CaffeBarP findCaffeBar(CaffeBarP first, char* caffeBarName);
int readFromFile(char* filename, CaffeBarP head);
int printListCaffeBar(CaffeBarP first);
int printTargetCaffeBar(CaffeBarP first, char* targetCafferBar);
int deleteVistors(PersonP first);
int deleteListCaffeBar(CaffeBarP head);

int main()
{
	CaffeBar head = { .caffeBarName = { 0 }, .visitors = NULL, .next = NULL };
	char targetCafferBar[MAX_NAME_LEN] = { 0 };

	if (readFromFile("kafici.txt", &head) != EXIT_SUCCESS)
	{
		perror("File could not be opened!\n");
		return FILE_OPEN_ERROR;
	}

	// Ispis liste kafica
	printf("Kafici:\n");
	printListCaffeBar(head.next);

	printf("\nUnesi ime kafica za prikaz posjetitelja: ");
	scanf("%31s", targetCafferBar);

	printTargetCaffeBar(head.next, targetCafferBar);

	// Oslobađanje memorije
	printf("\nOslobadanje memorije...\n");
	deleteListCaffeBar(&head);

	return EXIT_SUCCESS;
}

int addPersonToCaffeBarList(CaffeBarP caffeBar, PersonP newPerson)
{	
	PersonP currentPerson = NULL;

	if (caffeBar->visitors == NULL)
	{
		caffeBar->visitors = newPerson;
	}
	else
	{
		currentPerson = caffeBar->visitors;
		while (currentPerson->next != NULL)
			currentPerson = currentPerson->next;

		currentPerson->next = newPerson;
	}

	return EXIT_SUCCESS;
}

PersonP createPerson(char* firstName, char* lastName)
{
	PersonP newPerson = NULL;

	newPerson = (PersonP)malloc(sizeof(Person));
	if (newPerson == NULL)		// if (!newPerson)
	{
		perror("Memory allocation failed!\n");
		return NULL;
	}

	strcpy(newPerson->firstName, firstName);
	strcpy(newPerson->lastName, lastName);
	newPerson->next = NULL;

	return newPerson;
}


int insertSortedCaffeBar(CaffeBarP head, CaffeBarP newCaffeBar)
{
	CaffeBarP previous = head;
	CaffeBarP current = head->next;

	while (current != NULL && strcmp(current->caffeBarName, newCaffeBar->caffeBarName) < 0)
	{
		previous = current;
		current = current->next;
	}

	newCaffeBar->next = current;
	previous->next = newCaffeBar;

	return EXIT_SUCCESS;
}

CaffeBarP createcaffeBar(char* caffeBarName)
{
	CaffeBarP newcaffeBar = NULL;

	newcaffeBar = (CaffeBarP)malloc(sizeof(CaffeBar));
	if (newcaffeBar == NULL)		// if (!newcaffeBar)
	{
		perror("Memory allocation failed!\n");
		return NULL;
	}

	strcpy(newcaffeBar->caffeBarName, caffeBarName);
	newcaffeBar->visitors = NULL;
	newcaffeBar->next = NULL;
	
	return newcaffeBar;
}

CaffeBarP findCaffeBar(CaffeBarP first, char* caffeBarName)
{
	CaffeBarP current = first;

	while (current != NULL)
	{
		if (strcmp(current->caffeBarName, caffeBarName) == 0)
			return current;

		current = current->next;
	}

	return current;
}

int readFromFile(char* filename, CaffeBarP head) 
{
	FILE* filePointer = NULL;
	char firstName[MAX_NAME_LEN] = { 0 };
	char lastName[MAX_NAME_LEN] = { 0 };
	char caffeBarName[MAX_NAME_LEN] = { 0 };
	CaffeBarP newCaffeBar = NULL;
	PersonP newPerson = NULL;
	
	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!\n");
		return FILE_OPEN_ERROR;
	}
	

	while (fscanf(filePointer, "%31s %31s %31s", firstName, lastName, caffeBarName) == 3)
	{
		newCaffeBar = findCaffeBar(head->next, caffeBarName);
		if (newCaffeBar == NULL)	// if (!newCaffeBar)
		{
			newCaffeBar = createcaffeBar(caffeBarName);
			if (newCaffeBar == NULL)	// if (!newCaffeBar)
			{
				perror("Memory allocation failed!\n");
				fclose(filePointer);
				return MALLOC_ERROR;
			}
			insertSortedCaffeBar(head, newCaffeBar);
		}
		
		newPerson = createPerson(firstName, lastName);
		if (newPerson == NULL)	// if (!newPerson)
		{
			perror("Memory allocation failed!\n");
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		addPersonToCaffeBarList(newCaffeBar, newPerson);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printListCaffeBar(CaffeBarP first)
{
	CaffeBarP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return  EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("Ime kafica: %-31s\n", current->caffeBarName);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printTargetCaffeBar(CaffeBarP first, char* targetCafferBar)
{
	CaffeBarP caffeBar = first;
	PersonP visitor = NULL;
	int count = 0;

	caffeBar = findCaffeBar(first, targetCafferBar);
	if (caffeBar == NULL)
	{
		printf("Caffe Bar does not exist!");
		return EXIT_FAILURE;
	}

	visitor = caffeBar->visitors;

	printf("\nKafic: %-31s\n", caffeBar->caffeBarName);
	printf("Posjetitelji:\n");

	while (visitor != NULL)
	{
		printf("Ime: %-31s Prezime: %-31s\n", visitor->firstName, visitor->lastName);
		visitor = visitor->next;
		count++;
	}

	printf("Ukupno posjetitelja: %d\n", count);

	return EXIT_SUCCESS;
}

int deleteVistors(PersonP first)
{
	PersonP current = first;
	PersonP toDelete = NULL;  // ili temp svejedno

	while (current != NULL)
	{
		toDelete = current;
		current = current->next;
		free(toDelete);
	}
	return EXIT_SUCCESS;
}

int deleteListCaffeBar(CaffeBarP head)
{
	CaffeBarP previous = head;
	CaffeBarP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		deleteVistors(toDelete->visitors);
		free(toDelete);
	}

	return EXIT_SUCCESS;
}




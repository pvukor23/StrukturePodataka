/*
	Napisati program koji stvara dvije vezane liste, svaku od 20 elemenata, na nacin da se za svaki element
	liste generiraju dvije vrijednosti. Prva je vrijednost u opsegu od 100-130 (i u listi se ne smiju ponavljati
	vrijednosti), a druga je prioritet u opsegu od 1-5 (biti ce ponavljanja). Elementi u listi moraju biti
	sortirani prvo po prioritetu, pa po vrijednosti.

	Ocjena 2:
		Ispisati vezanu listu i pronaci vrijednosti koje se ponavljaju u obje liste (usporeduje se po value
		elementu), te ispisati te vrijednosti uz njihov prioritet i nakon toga ih izbrisati iz liste. Ispisati nove
		liste.

	Ocjena 3:
		Napraviti novu vezanu listu u koju ce se prebaciti sve te vrijednosti koje se ponavljaju (bez nove
		alokacije) i to sortirano po vrijednosti pa po prioritetu. Ispisati sve tri liste.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MALLOC_ERROR -1
#define EMPTY_LIST -2

#define N 20
#define MIN 100
#define MAX 130
#define PRIMIN 1
#define PRIMAX 5

struct _node;
typedef struct _node* NodeP;
typedef struct _node {
	int value;
	int priority;
	NodeP next;
} Node;

int inserSorted(NodeP head, NodeP newNode);
NodeP createNode(int value, int priority);
int valueExists(NodeP first, int value);
int generateList(NodeP head);
int printList(NodeP first);
int findAndPrintRepeatedValues(NodeP first1, NodeP first2);
//int delteRepeatedValues(NodeP head1, NodeP head2);
int insertSortedByValuePriority(NodeP newHead, NodeP current);
int moveRepeatsToNewList(NodeP head1, NodeP head2, NodeP newHead);
int deleteList(NodeP head);

int main()
{
	Node head1 = { .value = 0, .priority = 0, .next = NULL };
	Node head2 = { .value = 0, .priority = 0, .next = NULL };
	Node newHead = { .value = 0, .priority = 0, .next = NULL };

	srand(time(NULL));

	generateList(&head1);
	generateList(&head2);

	// Ispis prve liste
	printf("Prva lista:\n");
	printList(head1.next);

	// Ispis druge liste
	printf("Druga lista:\n");
	printList(head2.next);

	printf("\nVrijednosti koje se ponavljaju:\n");
	findAndPrintRepeatedValues(head1.next, head2.next);
/*
	printf("\nBrisanje ponavljaca...\n");
	delteRepeatedValues(&head1, &head2);

	// Ispis prve liste nakon brisanja
	printf("\nPrva lista nakon brisanja:\n");
	printList(head1.next);

	// Ispis druge liste nakon brisanja
	printf("\nDruga lista nakon brisanja:\n");
	printList(head2.next);
*/

	printf("\nPrebacivanje u novu listu...\n");
	moveRepeatsToNewList(&head1, &head2, &newHead);

	// Ispis nove liste nakon prebacivanja
	printf("\nNova lista nakon prebacivanja:\n");
	printList(newHead.next);

	// Oslobađanje prve memorije
	printf("\nOslobadanje prve memorije...\n");
	deleteList(&head1);

	// Oslobađanje druge memorije
	printf("\nOslobadanje druge memorije...\n");
	deleteList(&head2);

	return EXIT_SUCCESS;
}

int inserSorted(NodeP head, NodeP newNode)
{
	NodeP previous = head;
	NodeP current = head->next;

	while (current != NULL && (current->priority < newNode->priority || (current->priority == newNode->priority &&
		current->value < newNode->value)))
	{
		previous = current;
		current = current->next;
	}

	newNode->next = current;
	previous->next = newNode;

	return EXIT_SUCCESS;
}

NodeP createNode(int value, int priority)
{
	NodeP newNode = NULL;

	newNode = (NodeP)malloc(sizeof(Node));
	if (newNode == NULL) // if (!newNode)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	newNode->value = value;
	newNode->priority = priority;
	newNode->next = NULL;

	return newNode;
}

int valueExists(NodeP first, int value)
{
	NodeP current = first;

	while (current != NULL)
	{
		if (current->value == value)
			return EXIT_FAILURE;	// return 1;

		current = current->next;
	}

	return EXIT_SUCCESS;	// return 0;
}

int generateList(NodeP head)
{
	int count = 0;
	int value = 0;
	int priority = 0;
	NodeP newNode = NULL;

	while (count < N)
	{
		value = MIN + rand() % (MAX - MIN + 1);
		if (!valueExists(head->next, value))
		{

			priority = PRIMIN + rand() % (PRIMAX - PRIMIN + 1);

			newNode = createNode(value, priority);
			if (newNode == NULL) // if (!newNode)
			{
				perror("Memory allocation failed!");
				return MALLOC_ERROR;
			}
			inserSorted(head, newNode);
			count++;
		}
	}

	return EXIT_SUCCESS;
}

int printList(NodeP first)
{
	NodeP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("Vrijednost: %d Prioritet: %d\n", current->value, current->priority);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int findAndPrintRepeatedValues(NodeP first1, NodeP first2)
{
	NodeP current1 = first1;
	NodeP current2 = NULL;

	while (current1 != NULL)
	{
		current2 = first2;	// resetira se
		while (current2 != NULL)
		{
			if (current1->value == current2->value)
				printf("Vrijednost: %d Prioritet1: %d Prioritet2: %d\n", current1->value, current1->priority, current2->priority);
			current2 = current2->next;
		}
		current1 = current1->next;
	}

	return EXIT_SUCCESS;
}

//int delteRepeatedValues(NodeP head1, NodeP head2)
//{
//	NodeP previous1 = head1;
//	NodeP current1 = head1->next;
//	NodeP previous2 = NULL;
//	NodeP current2 = NULL;
//
//	while (current1 != NULL) 
//	{
//		previous2 = head2;
//		current2 = head2->next;
//		int found = 0;
//		while (current2 != NULL) 
//		{
//			if (current1->value == current2->value) 
//			{
//				// Brisanje iz liste 2
//				previous2->next = current2->next;
//				free(current2);
//
//				// Brisanje iz liste 1
//				previous1->next = current1->next;
//				free(current1);
//
//				found = 1;
//				break; // izlaz iz unutarnje liste i nastavak s novim current1
//			}
//			else 
//			{
//				previous2 = current2;
//				current2 = current2->next;
//			}
//		}
//
//		if (found == 0)	// if (!found)
//		{
//			previous1 = current1;
//			current1 = current1->next;
//		}
//		else 
//			current1 = previous1->next; // nastavak s elementom nakon obrisanog
//	}
//
//	return EXIT_SUCCESS;
//}

int insertSortedByValuePriority(NodeP newHead, NodeP current)
{
	NodeP newPrevious = newHead;
	NodeP newCurrent = newHead->next;

	while (newCurrent != NULL && newCurrent->priority < current->priority)
	{
		newPrevious = newCurrent;
		newCurrent = newCurrent->next;
	}

	current->next = newCurrent;
	newPrevious->next = current;

	return EXIT_SUCCESS;
}

int moveRepeatsToNewList(NodeP head1, NodeP head2, NodeP newHead)
{
	NodeP previous1 = head1;
	NodeP current1 = head1->next;
	NodeP previous2 = NULL;
	NodeP current2 = NULL;

	while (current1 != NULL)
	{
		previous2 = head2;
		current2 = head2->next;
		int found = 0;
		while (current2 != NULL)
		{
			if (current1->value == current2->value)
			{
				// Izvadi čvor iz liste 1
				previous1->next = current1->next;
				// Izvadi čvor iz liste 2
				previous2->next = current2->next;

				// Spoji liste, prvo curr1, pa curr2 u novu listu
				insertSortedByValuePriority(newHead, current1);
				insertSortedByValuePriority(newHead, current2);

				// Nastavi s elementom nakon obrisanog u listi 1
				current1 = previous1->next;
				found = 1;
				break;
			}
			else
			{
				previous2 = current2;
				current2 = current2->next;
			}
		}

		if (found == 0)		//if (!found)
		{
			previous1 = current1;
			current1 = current1->next;
		}
		else
			current1 = previous1->next;
	}

	return EXIT_SUCCESS;
}

int deleteList(NodeP head)
{
	NodeP previous = head;
	NodeP toDelete = NULL;	// ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous ->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

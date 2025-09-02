/* Napisati program koji za tri različita predmeta čita ime i prezime studenta te njegovu ocjenu.
Podaci o svakom predmetu se nalaze u odvojenoj datoteci i iz svake datoteke je potrebno kreirati vezanu listu i ispisati je.
a) Nakon toga potrebno je kreirati novu listu koja sadrži imena studenata koji su položili sva tri predmeta.
b) Nadopuniti zadatak pod a) tako da se ispiše nova vezana lista koja osim što sadrži imena studenata, sadrži i
njihovu prosječnu ocjenu iz ta tri predmeta. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MAX_NAME_LEN 32

struct _student;
typedef struct _student* StudentP;
typedef struct _student {
	char firstName[MAX_NAME_LEN];
	char lastName[MAX_NAME_LEN];
	int subject1;
	int subject2;
	int subject3;
	float average;
	StudentP next;
} Student;

int insertSorted(StudentP head, StudentP newStudent);
StudentP createStudent1(char* firstName, char* lastName, int subject1);
int readFromFile1(char* filename, StudentP head1);
StudentP createStudent2(char* firstName, char* lastName, int subject2);
int readFromFile2(char* filename, StudentP head2);
StudentP createStudent3(char* firstName, char* lastName, int subject3);
int readFromFile3(char* filename, StudentP head3);
int printList1(StudentP first);
int printList2(StudentP first);
int printList3(StudentP first);
StudentP createStudent(char* firstName, char* lastName, int subject1, int subject2, int subject3);
StudentP newList(StudentP head1, StudentP head2, StudentP head3, StudentP newHead);
int calculateAverageGrade(StudentP first);
int printList(StudentP first);
int deleteList1(StudentP head1);
int deleteList2(StudentP head2);
int deleteList3(StudentP head3);
int deleteNewList(StudentP newHead);

int main()
{
	Student head1 = { .firstName = { 0 }, .lastName = { 0 }, .subject1 = 0, .subject2 = 0, .subject3 = 0, .next = NULL };
	Student head2 = { .firstName = { 0 }, .lastName = { 0 }, .subject1 = 0, .subject2 = 0, .subject3 = 0, .next = NULL };
	Student head3 = { .firstName = { 0 }, .lastName = { 0 }, .subject1 = 0, .subject2 = 0, .subject3 = 0, .next = NULL };
	Student newHead = { .firstName = { 0 }, .lastName = { 0 }, .subject1 = 0, .subject2 = 0, .subject3 = 0, .next = NULL };

	if (readFromFile1("predmet1.txt", &head1) != EXIT_SUCCESS)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	if (readFromFile2("predmet2.txt", &head2) != EXIT_SUCCESS)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	if (readFromFile3("predmet3.txt", &head3) != EXIT_SUCCESS)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// Ispis liste 1
	printf("Strukture podataka:\n\n");
	printList1(head1.next);

	// Ispis liste 2
	printf("\nAlgoritmi:\n\n");
	printList2(head2.next);

	// Ispis liste 3
	printf("\nArhitekture:\n\n");
	printList3(head3.next);

	newList(&head1, &head2, &head3, &newHead);

	calculateAverageGrade(newHead.next);

	// Ispis nove liste 
	printf("\nProsli sve:\n\n");
	printList1(newHead.next);

	// Oslobađanje prve memorije
	printf("\nOslobadanje prve memorije...\n");
	deleteList3(&head1);

	// Oslobađanje prve memorije
	printf("\nOslobadanje prve memorije...\n");
	deleteList2(&head2);

	// Oslobađanje prve memorije
	printf("\nOslobadanje prve memorije...\n");
	deleteList3(&head3);

	// Oslobađanje nove memorije
	printf("\nOslobadanje nove memorije...\n");
	deleteNewList(&newHead);

	return EXIT_SUCCESS;
}


int insertSorted(StudentP head, StudentP newStudent)
{
	StudentP previous = head;
	StudentP current = head->next;

	while (current != NULL && (strcmp(current->lastName, newStudent->lastName) < 0 ||
		(strcmp(current->lastName, newStudent->lastName) == 0 &&
			strcmp(current->firstName, newStudent->firstName) < 0)))
	{
		previous = current;
		current = current->next;
	}

	newStudent->next = current;
	previous->next = newStudent;

	return EXIT_SUCCESS;
}

StudentP createStudent1(char* firstName, char* lastName, int subject1)
{
	StudentP newStudent = NULL;

	newStudent = (StudentP)malloc(sizeof(Student));
	if (newStudent == NULL)	// if (!newStudent)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	strcpy(newStudent->firstName, firstName);
	strcpy(newStudent->lastName, lastName);
	newStudent->subject1 = subject1;
	newStudent->subject2 = 1;
	newStudent->subject3 = 1;
	newStudent->next = NULL;

	return newStudent;
}

int readFromFile1(char* filename, StudentP head1)
{
	FILE* filePointer = NULL;
	char firstName[MAX_NAME_LEN] = { 0 };
	char lastName[MAX_NAME_LEN] = { 0 };
	int subject1 = 1;
	StudentP newStudent = NULL;
	char buffer[MAX_NAME_LEN] = { 0 };

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// preskoči prvi red (naziv predmeta)
	if (fgets(buffer, sizeof(buffer), filePointer) == NULL)
	{
		fclose(filePointer);
		fprintf(stderr, "Datoteka je prazna!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filePointer, "%s %s %d", lastName, firstName, &subject1) == 3)
	{
		newStudent = createStudent1(firstName, lastName, subject1);
		if (newStudent == NULL)	//if (!newStudent)
		{
			perror("Memory allocation failed!\n");
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		insertSorted(head1, newStudent);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

StudentP createStudent2(char* firstName, char* lastName, int subject2)
{
	StudentP newStudent = NULL;

	newStudent = (StudentP)malloc(sizeof(Student));
	if (newStudent == NULL)	// if (!newStudent)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	strcpy(newStudent->firstName, firstName);
	strcpy(newStudent->lastName, lastName);
	newStudent->subject1 = 1;
	newStudent->subject2 = subject2;
	newStudent->subject3 = 1;
	newStudent->next = NULL;

	return newStudent;
}

int readFromFile2(char* filename, StudentP head2)
{
	FILE* filePointer = NULL;
	char firstName[MAX_NAME_LEN] = { 0 };
	char lastName[MAX_NAME_LEN] = { 0 };
	int subject2 = 1;
	StudentP newStudent = NULL;
	char buffer[MAX_NAME_LEN] = { 0 };

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// preskoči prvi red (naziv predmeta)
	if (fgets(buffer, sizeof(buffer), filePointer) == NULL)
	{
		fclose(filePointer);
		fprintf(stderr, "Datoteka je prazna!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filePointer, "%s %s %d", lastName, firstName, &subject2) == 3)
	{
		newStudent = createStudent2(firstName, lastName, subject2);
		if (newStudent == NULL)	//if (!newStudent)
		{
			perror("Memory allocation failed!\n");
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		insertSorted(head2, newStudent);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

StudentP createStudent3(char* firstName, char* lastName, int subject3)
{
	StudentP newStudent = NULL;

	newStudent = (StudentP)malloc(sizeof(Student));
	if (newStudent == NULL)	// if (!newStudent)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	strcpy(newStudent->firstName, firstName);
	strcpy(newStudent->lastName, lastName);
	newStudent->subject1 = 1;
	newStudent->subject2 = 1;
	newStudent->subject3 = subject3;
	newStudent->next = NULL;

	return newStudent;
}

int readFromFile3(char* filename, StudentP head3)
{
	FILE* filePointer = NULL;
	char firstName[MAX_NAME_LEN] = { 0 };
	char lastName[MAX_NAME_LEN] = { 0 };
	int subject3 = 1;
	StudentP newStudent = NULL;
	char buffer[MAX_NAME_LEN] = { 0 };

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// preskoči prvi red (naziv predmeta)
	if (fgets(buffer, sizeof(buffer), filePointer) == NULL)
	{
		fclose(filePointer);
		fprintf(stderr, "Datoteka je prazna!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(filePointer, "%s %s %d", lastName, firstName, &subject3) == 3)
	{
		newStudent = createStudent3(firstName, lastName, subject3);
		if (newStudent == NULL)	//if (!newStudent)
		{
			perror("Memory allocation failed!\n");
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		insertSorted(head3, newStudent);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printList1(StudentP first)
{
	StudentP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("%s %s %d\n", current->firstName, current->lastName, current->subject1);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printList2(StudentP first)
{
	StudentP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("%s %s %d\n", current->firstName, current->lastName, current->subject2);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printList3(StudentP first)
{
	StudentP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("%s %s %d\n", current->firstName, current->lastName, current->subject3);
		current = current->next;
	}

	return EXIT_SUCCESS;
}


StudentP createStudent(char* firstName, char* lastName, int subject1, int subject2, int subject3)
{
	StudentP newStudent = NULL;

	newStudent = (StudentP)malloc(sizeof(Student));
	if (newStudent == NULL)	// if (!newStudent)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	strcpy(newStudent->firstName, firstName);
	strcpy(newStudent->lastName, lastName);
	newStudent->subject1 = subject1;
	newStudent->subject2 = subject2;
	newStudent->subject3 = subject3;
	newStudent->next = NULL;

	return newStudent;
}

StudentP newList(StudentP head1, StudentP head2, StudentP head3, StudentP newHead)
{
	StudentP current1 = head1->next;
	StudentP previous1 = head1;
	StudentP current2 = head2->next;
	StudentP previous2 = head2;
	StudentP current3 = head3->next;
	StudentP previous3 = head3;
	StudentP newStudent = NULL;

	while (current1 != NULL)
	{
		while (current2 != NULL)
		{
			if ((strcmp(current1->firstName, current2->firstName) == 0) && (strcmp(current1->lastName, current2->lastName) == 0))
			{
				while (current3 != NULL)
				{
					if ((strcmp(current1->firstName, current3->firstName) == 0) && (strcmp(current1->lastName, current3->lastName) == 0))
					{
						newStudent = createStudent(current1->firstName, current1->lastName, current1->subject1, current2->subject2, current3->subject3);
						if (newStudent == NULL)	//if (!newStudent)
						{
							perror("Memory allocation failed!\n");
							return NULL;
						}
						insertSorted(newHead, newStudent);
						previous3 = current3;
						break;
					}
					current3 = current3->next;
				}
				previous2 = current2;
				break;
			}
			current2 = current2->next;
		}
		current1 = current1->next;
		current2 = previous2->next;
		current3 = previous3->next;
	}

	return newHead;
}

int calculateAverageGrade(StudentP first)
{
	StudentP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		float sum = 0.0;
		sum = current->subject1 + current->subject2 + current->subject3;
		current->average = sum / 3.0;
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printList(StudentP first)
{
	StudentP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("%s %s %d\n", current->firstName, current->lastName, current->average);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int deleteList1(StudentP head1)
{
	StudentP previous = head1;
	StudentP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

int deleteList2(StudentP head2)
{
	StudentP previous = head2;
	StudentP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

int deleteList3(StudentP head3)
{
	StudentP previous = head3;
	StudentP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

int deleteNewList(StudentP newHead)
{
	StudentP previous = newHead;
	StudentP toDelete = NULL;  // ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;    // previous->next = toDelete->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;
}
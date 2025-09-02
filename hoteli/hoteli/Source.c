/* Napisati program koji iz datoteke "hoteli.txt" čita popis hotela te kreira vezanu listu sortiranu po datumu izgradnje hotela. 
Ispisati sortiranu vezanu listu. Izračunati srednju vrijednost godišnjih prihoda svih hotela pa ispisati listu tako da se iz 
prethodne liste izbace hoteli s prihodom manjim od izračunate srednje vrijednosti. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MAX_NAME_LEN 32

struct _date;
typedef struct _date* DateP;
typedef struct _date {
	int day;
	int month;
	int year;
} Date;

struct _hotel;
typedef struct _hotel* HotelP;
typedef struct _hotel {
	char name[MAX_NAME_LEN];
	float income;			
	DateP dateOfBuilding;	
	HotelP next;
} Hotel;

int compareDates(DateP d1, DateP d2);
int insertSorted(HotelP head, HotelP newTemp);
HotelP createHotel(char* name, float income, DateP date);
int readFromFile(char* filename, HotelP head);
int printList(HotelP first);
float calculateAverageIncome(HotelP first);
int deleteBelowAverage(HotelP head, float averageIncome);
int deleteList(HotelP head);

int main()
{
	Hotel head = { .name = { 0 }, .income = 0.0, .dateOfBuilding = NULL, .next = NULL };

	float averageIncome = 0.0;
	int idToMove = 0;

	if (readFromFile("hoteli.txt", &head) != EXIT_SUCCESS)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// Ispis liste
	printf("Originalna lista:\n");
	printList(head.next);

	averageIncome = calculateAverageIncome(head.next);
	printf("\nProsjecno godisnji profit svih hotela je: %.2f milijuna eura", averageIncome);

	deleteBelowAverage(&head, averageIncome);

	printf("\nLista nakon brisanja hotela s manjim godisnjim prihodom od srednje vrijednosti svih hotela:\n");
	printList(head.next);

	// Oslobađanje memorije
	printf("\nOslobadanje memorije...\n");
	deleteList(&head);

	return EXIT_SUCCESS;
}

int compareDates(DateP d1, DateP d2)
{
	if (d1->year != d2->year)
		return d1->year - d2->year;
	else if (d1->month != d2->month)
		return d1->month - d2->month;
	else
		return d1->day - d2->day;
}

int insertSorted(HotelP head, HotelP newHotel)
{
	HotelP previous = head;
	HotelP current = head->next;

	while (current != NULL && compareDates(current->dateOfBuilding, newHotel->dateOfBuilding) < 0)
	{
		previous = current;
		current = current->next;
	}

	newHotel->next = current;
	previous->next = newHotel;

	return EXIT_SUCCESS;
}

HotelP createHotel(char* name, float income, DateP date)
{
	HotelP newHotel = NULL;

	newHotel = (HotelP)malloc(sizeof(Hotel));
	if (newHotel == NULL)	// if (!newHotel)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	strcpy(newHotel->name, name);
	newHotel->income = income;
	newHotel->dateOfBuilding = date;
	newHotel->next = NULL;

	return newHotel;
}

int readFromFile(char* filename, HotelP head)
{
	FILE* filePointer = NULL;
	int day = 0;
	int month = 0;
	int year = 0;
	char name[MAX_NAME_LEN] = { 0 };
	float income = 0.0;
	DateP date = NULL;
	HotelP newHotel = NULL;

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	while (fscanf(filePointer, "%s %d.%d.%d. %f", name, &day, &month, &year, &income) == 5)
	{

		date = (DateP)malloc(sizeof(Date));
		if (date == NULL)	// if (!date)
		{
			perror("Memory allocation failed!");
			deleteList(head);
			fclose(filePointer);
			return MALLOC_ERROR;
		}

		date->day = day;
		date->month = month;
		date->year = year;

		newHotel = createHotel(name, income, date);
		if (newHotel == NULL)	// if (!newHotel)
		{
			perror("Memory allocation failed!");
			free(date);
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		insertSorted(head, newHotel);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printList(HotelP first)
{
	HotelP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("\nDatum: %d.%d.%d.\nIme: %s\nGodisnji profit: %.2f milijuna eura\n",
			current->dateOfBuilding->day,
			current->dateOfBuilding->month,
			current->dateOfBuilding->year,
			current->name,
			current->income);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

float calculateAverageIncome(HotelP first)
{
	HotelP current = first;
	int count = 0;
	float sum = 0.0;

	while (current != NULL)
	{
		sum += current->income;
		count++;
		
		current = current->next;
	}

	if (count > 0)
		return sum / count;
	else
		return 0.0;
}

int deleteBelowAverage(HotelP head, float averageIncome)
{
	HotelP previous = head;
	HotelP current = head->next;

	while (current != NULL)
	{
		if (current->income < averageIncome)
		{
			previous->next = current->next;
			free(current->dateOfBuilding);
			free(current);
			current = previous->next;	// oslobodili smo memoriju na koju current pokazuje	
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}

	return EXIT_SUCCESS;
}

int deleteList(HotelP head)
{
	HotelP previous = head;
	HotelP toDelete = NULL;	// ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;	// previous ->next = toDelete->next;
		free(toDelete->dateOfBuilding);			//oslobodit datum
		free(toDelete);
	}

	return EXIT_SUCCESS;
}
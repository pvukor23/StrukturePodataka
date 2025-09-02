/*
	Napisati program koji iz datoteke "temperature.txt" čita podatke o
	temperaturama Jadranskog mora koje su izmjerene na određenoj
	dubini određenog datuma. U datoteci je prvo zapisan datum, pa
	dubina mjerenja, pa temperatura. Podatke je potrebno spremiti u 
	jednostruko vezanu listu sortirano po datumu mjerenja te ispisati
	listu.

	Ocjena 2: Izračunati prosječnu temperaturu Jadranskog mora na
	dubini većoj od 50 metara u 2023. godini. SVA mjerenja čija je
	temperatura manja od prosječne potrebne je izbrisati iz vezane
	liste te potom ispisati listu.

	Ocjena 3: Za svako mjerenje generirati jedinstveni ID u opsegu od
	500 do 610, a zatim unijet jedan ID i prebaciti mjerenje koje ima
	taj ID na početak vezane liste. Potom ispisati novu listu.


	U izradi zadatka mogu se koristiti strukture:

	struct _date;
	typedef struct _date *DateP;
	typedef struct _date {
		int day;
		int month;
		int year;
	} Date;


	struct _temperature;
	typedef struct _temperature *TemperatureP;
	typedef struct _temperature {			
		int depth;
		DateP dateOfMeasurement;	
		TemperatureP next;
	} Temperature;


	NAPOMENA Za bilo koju ocjenu potrebno je pri završetku programa
	osloboditi svu dinamički alociranu memoriju i
	zatvoriti sve datoteke.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define EMPTY_LIST -3

#define MIN_ID 500
#define MAX_ID 610

struct _date;
typedef struct _date* DateP;
typedef struct _date {
	int day;
	int month;
	int year;
} Date;

struct _temperature;
typedef struct _temperature* TemperatureP;
typedef struct _temperature	{
	int ID;						// dodano za 3
	int depth;
	float temperature;			// dodao msn da je krivo na papiru
	DateP dateOfMeasurement;	// Pokazivač na `Date`
	TemperatureP next;
} Temperature;


TemperatureP createTemperature(int depth, float temperature,  DateP date);
int insertSorted(TemperatureP head, TemperatureP newTemp);
int readFromFile(char* filename, TemperatureP head);
int printList(TemperatureP first);
int printListWithID(TemperatureP first);
float calculateAverageTemperature(TemperatureP first);
int deleteBelowAverage(TemperatureP head, float averageTemperature);
int generateIDs(TemperatureP first);
//int generateIDs(TemperatureP first);
int moveMeasurementToFrontById(TemperatureP head, int id);
int deleteList(TemperatureP head);

int main()
{
	Temperature head = { .ID = 0, .depth = 0, .temperature = 0.0, .dateOfMeasurement = NULL, .next = NULL };

	float averageTemperature = 0.0;
	int idToMove = 0;

	srand(time(NULL));

	if (readFromFile("temperature.txt", &head) != EXIT_SUCCESS)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	// Ispis liste
	printf("Originalna lista:\n");
	printList(head.next);

	averageTemperature = calculateAverageTemperature(head.next);
	printf("\nProsjecna temperatura na dubinama > 50 m u 2023. godini: %.2f C\n", averageTemperature);

	deleteBelowAverage(&head, averageTemperature);

	printf("\nLista nakon brisanja temperatura manje od prosjecne na > 50m dubine u 2023. godini:\n");
	printList(head.next);

	generateIDs(head.next);

	/*generateIDs(head.next;*/

	printf("\nLista nakon dodjele jedinstvenog ID-a svakom mjerenju:\n");
	printListWithID(head.next);

	// Prebacivanje mjerenja na pocetak liste po ID-u
	do {
		printf("\nUnesite ID mjerenja za prebacivanje na pocetak liste (od 500 do 610): ");
		scanf("%d", &idToMove);
		if (idToMove < 500 || idToMove > 610)
			printf("Neispravan unos, unesite ID mjerenja iz raspona 500-610.\n");
	} while (idToMove < 500 || idToMove > 610);

	moveMeasurementToFrontById(&head, idToMove);

	// Ispis liste nakon prebacivanja odredenog mjerenja na pocetak liste
	printf("\nNova lista:\n");
	printListWithID(head.next);

	// Oslobađanje memorije
	printf("\nOslobadanje memorije...\n");
	deleteList(&head);

	return EXIT_SUCCESS;
}

//int insertSorted(TemperatureP head, TemperatureP newTemperature)
//{
//	TemperatureP current = head->next;
//	TemperatureP prev = head;
//
//	while (current != NULL && (current->dateOfMeasurement->year < newTemperature->dateOfMeasurement->year ||
//		(current->dateOfMeasurement->year == newTemperature->dateOfMeasurement->year &&
//			current->dateOfMeasurement->month < newTemperature->dateOfMeasurement->month) ||
//		(current->dateOfMeasurement->year == newTemperature->dateOfMeasurement->year &&
//			current->dateOfMeasurement->month == newTemperature->dateOfMeasurement->month &&
//			current->dateOfMeasurement->day < newTemperature->dateOfMeasurement->day)))
//	{
//		prev = current;
//		current = current->next;
//	}
//
//	newTemperature->next = current;
//	prev->next = newTemperature;
//
//	return EXIT_SUCCESS;
//}

int compareDates(DateP d1, DateP d2)
{
	if (d1->year != d2->year)
		return d1->year - d2->year;
	else if (d1->month != d2->month)
		return d1->month - d2->month;
	else
		return d1->day - d2->day;
}

int insertSorted(TemperatureP head, TemperatureP newTemperature)
{
	TemperatureP previous = head;
	TemperatureP current = head->next;

	while (current != NULL && compareDates(current->dateOfMeasurement, newTemperature->dateOfMeasurement) < 0)
	{
		previous = current;
		current = current->next;
	}

	newTemperature->next = current;
	previous->next = newTemperature;

	return EXIT_SUCCESS;
}

TemperatureP createTemperature(int depth, float temperature, DateP date)
{
	TemperatureP newTemperature = NULL;

	newTemperature = (TemperatureP)malloc(sizeof(Temperature));
	if (newTemperature == NULL)	// if (!newTemperature)
	{
		perror("Memory allocation failed!");
		return NULL;
	}

	newTemperature->depth = depth;
	newTemperature->temperature = temperature;
	newTemperature->dateOfMeasurement = date;
	newTemperature->next = NULL;

	return newTemperature;
}

int readFromFile(char* filename, TemperatureP head)
{
	FILE* filePointer = NULL;
	int day = 0;
	int month = 0;
	int year = 0;
	int depth = 0;
	float temperature = 0.0;
	DateP date = NULL;
	TemperatureP newTemperature = NULL;

	filePointer = fopen(filename, "r");
	if (filePointer == NULL)	// if (!filePointer)
	{
		perror("File could not be opened!");
		return FILE_OPEN_ERROR;
	}

	while (fscanf(filePointer, "%d.%d.%d. %d %f", &day, &month, &year, &depth, &temperature) == 5)
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

		newTemperature = createTemperature(depth, temperature, date);
		if(newTemperature == NULL)	// if (!newTemperature)
		{
			perror("Memory allocation failed!");
			free(date);	
			fclose(filePointer);
			return MALLOC_ERROR;
		}
		insertSorted(head, newTemperature);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printList(TemperatureP first)
{	
	TemperatureP current = first;
	
	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("\nDatum: %d.%d.%d.\nDubina: %d m\nTemperatura: %.2f C\n",
			current->dateOfMeasurement->day,
			current->dateOfMeasurement->month,
			current->dateOfMeasurement->year,
			current->depth,
			current->temperature);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printListWithID(TemperatureP first)
{
	TemperatureP current = first;

	if (current == NULL)
	{
		printf("Empty list!\n");
		return EMPTY_LIST;
	}

	while (current != NULL)
	{
		printf("\nID: %d\nDatum: %d.%d.%d.\nDubina: %d m\nTemperatura: %.2f C\n",
			current->ID,
			current->dateOfMeasurement->day,
			current->dateOfMeasurement->month,
			current->dateOfMeasurement->year,
			current->depth,
			current->temperature);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

float calculateAverageTemperature(TemperatureP first)
{
	TemperatureP current = first;
	int count = 0;
	float sum = 0.0;

	while (current != NULL)
	{
		if (current->depth > 50 && current->dateOfMeasurement->year == 2023)
		{
			sum += current->temperature;
			count++;
		}
		current = current->next;
	}

	if (count > 0)
		return sum / count;
	else
		return 0.0;
}

int deleteBelowAverage(TemperatureP head, float averageTemperature)
{
	TemperatureP previous = head;
	TemperatureP current = head->next;

	while (current != NULL)
	{
		if (current->temperature < averageTemperature)
		{
			previous->next = current->next;
			free(current->dateOfMeasurement);
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

int generateIDs(TemperatureP first)
{
	TemperatureP current = first;
	int usedIDs[MAX_ID - MIN_ID + 1] = { 0 };
	int count = 0;
	int id = 0;

	while (current != NULL && count < (MAX_ID - MIN_ID + 1))
	{
		do {
			id = MIN_ID + rand() % (MAX_ID - MIN_ID + 1);
		} while (usedIDs[id - MIN_ID]);	// ponavljaj dok je generirani ID vec iskoristen, jer se iskoristen postavlja na 1 pa while(1)

		usedIDs[id - MIN_ID] = 1;	// postavljam da je taj ID iskoristen
		current->ID = id;
		count++;
		current = current->next;
	}

	if (current != NULL)
	{
		fprintf(stderr, "Not enough unique IDs for all measurement!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


/* int generateIDs(TemperatureP first)
{
	TemperatureP current = first;
	int id = 500;

	while (current && current->ID <= 610)
	{
		current->ID = id++;
		current = current->next;
	}

	return EXIT_SUCCESS;

} */

int moveMeasurementToFrontById(TemperatureP head, int id)
{
	TemperatureP previous = head;
	TemperatureP current = head->next;

	while (current != NULL)
	{
		if (current->ID == id)
		{
			previous->next = current->next;
			current->next = head->next;
			head->next = current;
			return EXIT_SUCCESS;
		}
		previous = current;
		current = current->next;
	}

	printf("Measurement with ID %d not found.\n", id);
	return EXIT_FAILURE;  
}


int deleteList(TemperatureP head)
{
	TemperatureP previous = head;
	TemperatureP toDelete = NULL;	// ili temp svejedno

	while (previous->next != NULL)
	{
		toDelete = previous->next;
		previous->next = previous->next->next;	//// previous ->next = toDelete->next;
		free(toDelete->dateOfMeasurement); //oslobodit datum
		free(toDelete);
	}

	return EXIT_SUCCESS;
}

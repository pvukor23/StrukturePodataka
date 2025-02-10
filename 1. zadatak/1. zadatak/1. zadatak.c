/*
	1.	Napisati program koji prvo pročita koliko redaka ima datoteka, tj.koliko ima studenata
		zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
		studenata(ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
		prezime, apsolutni i relativni broj bodova.

		Napomena: Svaki redak datoteke sadrži ime i prezime Studenta, te broj bodova na kolokviju.
				  relatvan_br_bodova = br_bodova / max_br_bodova * 100
*/

#define _CRT_SECURE_NO_WARNINGS	// Isključuje sigurnosna upozorenja na funkcije poput scanf, fgets itd.

#include <stdio.h>	// Uključivanje standardne biblioteke za ulaz/izlaz (npr. printf, fscanf)
#include <stdlib.h>	// Uključivanje standardne biblioteke za rad s memorijom (npr. malloc, free)

#define MAX_LINE 1024	// Maksimalna veličina buffera za čitanje linije iz datoteke
#define MAX_LENGTH 128	// Maksimalna veličina stringa za ime i prezime
#define MAX_POINTS 15	// Maksimalni broj bodova koje student može ostvariti na kolokviju

#define FILE_ERROR_OPEN -1 // Konstanta koja označava grešku pri otvaranju datoteke
#define MALLOC_ERROR -2	// Konstanta koja označava grešku pri alokaciji memorije koristeći malloc funkciju

#define RESET   "\033[0m"	// ANSI escape sekvenca za resetiranje svih stilova i boja na zadane postavke terminala
#define RED     "\033[31m"	// ANSI escape sekvenca za postavljanje boje teksta na crvenu
#define BOLD    "\033[1m"	// ANSI escape sekvenca za podebljavanje teksta (bold stil)

typedef struct _Student {	// Definicija strukture za studenta
	char name[MAX_LENGTH];	// Polje za ime studenta (do 128 znakova)
	char surname[MAX_LENGTH];	// Polje za prezime studenta (do 128 znakova)
	double points;	// Bodovi studenta na kolokviju
} Student;	// Alias za strukturu _Student, sada možemo koristiti 'Student' kao tip podataka

// Prototipi funkcija koje će biti implementirane
int readNoRowsInFile();	// Funkcija koja vraća broj studenata (redaka u datoteci)
Student* allocateMemoryAndReadStudents(int noStudents);	// Funkcija koja alocira memoriju za studente i čita podatke iz datoteke
double calculateRelativePoints(double points);	// Funkcija koja izračunava relativne bodove
int showStudents(int noStudents, Student* students);	// Funkcija koja ispisuje podatke svih studenata

int main()
{
	int noRows = 0;	// Varijabla za broj redaka	(studenata) u datoteci 
	Student* students = NULL;	// Pokazivač na niz studenata 

	noRows = readNoRowsInFile();	// Pozivanje funkcije koja vraća broj studenata iz datoteke
	if (noRows > 0)	// Ako je broj redaka (studenata) veći od 0
	{
		students = allocateMemoryAndReadStudents(noRows);	// Pozivanje funkcije koja dinamički alocira memoriju za niz studenata i učitava podatke o njima
		if (!students)	// Provjera je li alokacija memorije uspješna (ili students == NULL)
		{
			printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
			return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije	
		}
		showStudents(noRows, students);	// Ispisivanje podataka svih studenata

		free(students);	// Oslobađanje prethodne alocirane memorije
	}

	return EXIT_SUCCESS;    // Kraj programa, signalizira uspješan završetak
}

int readNoRowsInFile()
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	char buffer[MAX_LINE] = { 0 };	// Buffer za privremeno pohranjivanje pročitanog reda
	int rowCounter = 0;	// Brojač redaka u datoteci

	filePointer = fopen("students.txt", "r");	// Otvaranje datoteke "students.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n"); // Ispis poruke o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;  // Povratna vrijednost koja označava grešku pri otvaranju datoteke
	}

	/*

	Ne koristiti feof() jer može uzrokovati greške pri čitanju, posebno zbog praznih redaka na kraju datoteke!!!

	while (!feof(filePointer))	// Iteracija kroz datoteku do kraja
	{
		fgets(buffer, MAX_LINE, filePointer);	// Čita redak iz datoteke te ga sprema u buffer
		rowCounter++;	// Povećava brojač redaka nakon svakog pročitanog reda
	}

	*/

	while (fgets(buffer, MAX_LINE, filePointer) != NULL)	// Čita jedan redak iz datoteke i pohranjuje ga u buffer dok ne dođe do kraja datoteke (NULL označava kraj datoteke)
		rowCounter++;	// Povećava brojač redaka nakon svakog uspješno pročitanog reda
	
	fclose(filePointer);	// Zatvaranje datoteke

	return rowCounter;	// Vraća broj redaka u datoteci (broj studenata)
}

Student* allocateMemoryAndReadStudents(int noStudents)
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	Student* students = NULL;	// Pokazivač na dinamički alocirani niz studenata tipa Student
	int i = 0;	// Indeks za iteraciju kroz niz studenata

	filePointer = fopen("students.txt", "r");	// Otvaranje datoteke "students.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n");	// Ispis poruke o grešci ako datoteka nije otvorena
		return NULL;	// Povratna vrijednost označava grešku pri otvaranju datoteke, NULL a ne FILE_ERROR_OPEN jer funckija vraća pokazivač
	}

	students = (Student*)malloc(noStudents * sizeof(Student));	// Dinamički alocira memoriju za niz studenata, veličine 'noStudents'
	if (!students)	// Provjera je li alokacija memorije uspješna (ili students == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		fclose(filePointer);	// Zatvaranje datoteke
		return NULL;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije, NULL a ne MALLOC_ERROR jer funckija vraća pokazivač
	}

	for (i = 0; i < noStudents; i++)	// Iteracija kroz sve studente u datoteci 
	{
		if (fscanf(filePointer, " %-10s %-15s %lf", students[i].name, students[i].surname, &students[i].points) != 3)	// Provjerava jesu li uspješno pročitane 3 vrijednosti (ime, prezime i bodove) iz datoteke koje se spremaju u trenutnu strukturu 'Student'
		{
			printf("Error reading data from file!\n");	// Ispis poruke o grešci ako čitanje podataka iz datoteke nije uspješan
			free(students);	// Oslobađanje prethodne alocirane memorije
			fclose(filePointer);	// Zatvaranje datoteke
			return NULL;	// Povratna vrijednost koja označava grešku pri čitanju podataka iz datoteke, NULL a ne SCANF_ERROR jer funckija vraća pokazivač
		}		
	}

	fclose(filePointer);	// Zatvaranje datoteke

	return students;	// Vraća pokazivač na alocirani niz studenata
}

double calculateRelativePoints(double points)
{
	if (points < 0 || points > MAX_POINTS)	// Provjera jesu li bodovi unutar dopuštenog raspona (0 do MAX_POINTS)
	{
		printf(BOLD RED "\nInvalid points: %.2lf. Points must be between 0 and %d.\n" RESET, points, MAX_POINTS);	// Ispis poruke o grešci u crvenoj boji s podebljanim stilom ako su uneseni bodovi izvan dopuštenog raspona (0 do MAX_POINTS)
		return 0;	// Vraća 0 kao signal greške, jer bodovi nisu u valjanom rasponu
	} 
	else	// Ako su bodovi unutar raspona, izračunava relativni broj bodova
		return ((points / MAX_POINTS) * 100);  // Relativni broj bodova se izračunava kao (bodovi studenta / maksimalni broj bodova) * 100
}			

int showStudents(int noStudents, Student* students)
{
	int i = 0;	// Indeks za iteraciju kroz niz studenata

	for (i = 0; i < noStudents; i++)	// Iteracija kroz sve studente
	{
		printf("Name: %-10s\tSurname: %-10s\t Absolute points: %.2lf\t\t Relative points: %.2lf%%\t\n",	// Ispis podataka svakog studenta
			students[i].name,	// Ime studenta (minimalna širina polja za ispisivanje stringa je 10 znakova, poravnato ulijevo)  
			students[i].surname,	// Prezime studenta (minimalna širina polja za ispisivanje stringa je 10 znakova, poravnato ulijevo)
			students[i].points,	// Apsolutni broj bodova (na 2 decimale)
			calculateRelativePoints(students[i].points));	// Pozivanje funkcije za izračun relativnih broja bodova, izražen kao postotak (na 2 decimale) 
	}

	return EXIT_SUCCESS;    // Kraj funkcije, signalizira uspješan završetak
}



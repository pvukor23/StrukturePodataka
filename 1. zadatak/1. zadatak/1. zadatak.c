/*
	1.	Napisati program koji prvo pročita koliko redaka ima datoteka, tj.koliko ima studenata
		zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
		studenata(ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
		prezime, apsolutni i relativni broj bodova.

		Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
				  relatvan_br_bodova = br_bodova / max_br_bodova * 100 
*/

#define _CRT_SECURE_NO_WARNINGS	// Isključuje sigurnosna upozorenja na funkcije poput scanf, fgets itd.

#include <stdio.h>	// Uključivanje standardne biblioteke za ulaz/izlaz (npr. printf, fscanf)
#include <stdlib.h>	// Uključivanje standardne biblioteke za rad s memorijom (npr. malloc, free)

#define MAX_LENGTH 128	// Maksimalna veličina stringa za ime i prezime
#define MAX_SIZE 1024	// Maksimalna veličina buffera za čitanje linije iz datoteke
#define MAX_POINTS 15	// Maksimalni broj bodova koje student može ostvariti na kolokviju
#define FILE_ERROR_OPEN -1 // Konstanta koja označava grešku pri otvaranju datoteke
#define MALLOC_ERROR -2	// Konstanta koja označava grešku pri alokaciji memorije koristeći malloc funkciju
#define SCANF_ERROR	-3	// Konstanta koja označava grešku pri čitanju podataka iz datoteke koristeći fscanf

typedef struct _student {	// Definicija strukture za studenta
	char name[MAX_LENGTH];	// Polje za ime studenta (do 128 znakova)
	char surname[MAX_LENGTH];	// Polje za prezime studenta (do 128 znakova)
	double points;	// Bodovi studenta na kolokviju
} student;	// Alias za strukturu _student, sada možemo koristiti 'student' kao tip podataka

// Prototipi funkcija koje će biti implementirane
int readNoRowsInFile();	// Funkcija koja vraća broj studenata (redaka u datoteci)
student* allocateMemoryAndReadStudents(int noStudents);	// Funkcija koja alocira memoriju za studente i čita podatke iz datoteke
double calculateRelativePoints(double points);	// Funkcija koja izračunava relativne bodove
int showStudents(int noStudents, student* students);	// Funkcija koja ispisuje podatke svih studenata

int main()
{
	int noRows = 0;	// Varijabla za broj redaka u datoteci (studenti)
	student* students = NULL;	// Pokazivač na dinamički alocirane studente

	noRows = readNoRowsInFile();	// Pozivanje funkcije koja vraća broj studenata iz datoteke
	if (noRows > 0)	// Ako je broj studenata veći od 0
	{
		students = allocateMemoryAndReadStudents(noRows);	// Alociraj memoriju i učitaj podatke o studentima
		showStudents(noRows, students);	// Ispisivanje podataka svih studenata

		free(students);	// Oslobađanje alocirane memorije
	}

	return 0;	// Kraj programa
}

int readNoRowsInFile()
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	char buffer[MAX_SIZE] = { 0 };	// Buffer za privremeno pohranjivanje pročitanog reda
	int rowCounter = 0;	// Brojač redaka u datoteci

	filePointer = fopen("students.txt", "r");	// Otvaranje datoteke "students.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n"); // Ispisuje poruku o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;  // Povratna vrijednost označava grešku pri otvaranju datoteke
	}
	while (!feof(filePointer))	// Iteracija kroz datoteku do kraja
	{
		fgets(buffer, MAX_SIZE, filePointer);	// Čita redak iz datoteke te ga sprema u buffer
		rowCounter++;	// Povećava brojač redaka nakon svakog pročitanog reda
	}

	fclose(filePointer);	// Zatvaranje datoteke

	return rowCounter;	// Vraća broj redaka u datoteci (broj studenata)
}

student* allocateMemoryAndReadStudents(int noStudents)
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	student* students = NULL;	// Pokazivač na dinamički alocirani niz struktura tipa student
	int i = 0;	// Indeks za iteraciju kroz niz studenata

	filePointer = fopen("students.txt", "r");	// Otvaranje datoteke "students.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n");	// Ispisuje poruku o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;	// Povratna vrijednost označava grešku pri otvaranju datoteke
	}

	students = (student*)malloc(noStudents * sizeof(student));	// Dinamički alocira memoriju za niz struktura student, veličine 'noStudents'
	if (!students)	// Provjera je li alokacija memorije uspješna (ili students == NULL)
	{
		printf("Error allocating memory!\n");	// Ispisuje poruku o grešci ako alokacija nije uspjela
		return MALLOC_ERROR;	// Povratna vrijednost označava grešku pri alokaciji memorije pomoću malloc funkcije
	}

	while (!feof(filePointer))	// Iteracija kroz datoteku do kraja
	{
		if (fscanf(filePointer, " %s %s %lf", students[i].name, students[i].surname, &students[i].points) != 3)	// Provjerava je li uspješno pročitao 3 vrijednosti (ime, prezime i bodove) iz datoteke te ih sprema u trenutnu strukturu student
			return SCANF_ERROR;	// Povratna vrijednost označava grešku pri čitanju podataka iz datoteke
		i++;	// Povećava indeks ako je čitanje podataka bilo uspješno kako bi prešao na sljedećeg studenta
	}

	fclose(filePointer);	// Zatvaranje datoteke

	return students;	// Vraća pokazivač na alocirani niz studenata
}

double calculateRelativePoints(double points)
{
	return ((points / MAX_POINTS) * 100);  // Relativni broj bodova se izračunava kao (bodovi studenta / maksimalni broj bodova) * 100
}

int showStudents(int noStudents, student* students)
{
	int i;	// Indeks za iteraciju kroz niz studenata

	for (i = 0; i < noStudents; i++)	// Iteracija kroz sve studente
	{
		printf("Name: %-10s\t Surname: %-10s\t Absolute points: %.2lf\t\t Relative points: %.2lf%\t\n",	// Ispisivanje podataka svakog studenta
			students[i].name,	// Ime studenta (minimalna širina polja za ispisivanje stringa je 10 znakova, poravnato ulijevo)  
			students[i].surname,	// Prezime studenta (minimalna širina polja za ispisivanje stringa je 10 znakova, poravnato ulijevo)
			students[i].points,	// Apsolutni broj bodova (na 2 decimale)
			calculateRelativePoints(students[i].points));	// Relativni broj bodova koji se izračunava pomoću funkcije calculateRelativePoints (na 2 decimale)
	}

	return 0;	// Kraj programa
}

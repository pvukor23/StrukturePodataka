/*
	3.	Prethodnom zadatku dodati funkcije:
			A. dinamički dodaje novi element iza određenog elementa,
			B. dinamički dodaje novi element ispred određenog elementa,
			C. sortira listu po prezimenima osoba,
			D. upisuje listu u datoteku,
			E. čita listu iz datoteke.
*/

#define _CRT_SECURE_NO_WARNINGS	// Isključuje sigurnosna upozorenja na funkcije poput scanf, fgets itd.

#include <stdio.h>	// Uključivanje standardne biblioteke za ulaz/izlaz (npr. printf, fscanf)
#include <stdlib.h>	// Uključivanje standardne biblioteke za rad s memorijom (npr. malloc, free)
#include <string.h>	// Uključivanje standardne biblioteke za rad s manipulacijom stringova (npr. strcpy, strcmp, strlen)
#include <ctype.h>	// Uključivanje standardne biblioteke za rad s funkcijama za manipulaciju karakterima (npr. isalpha, tolower, toupper)

#define MAX_LINE 1024	// Maksimalna veličina buffera za čitanje linije iz datoteke
#define MAX_LENGTH 128	// Maksimalna veličina stringa za ime i prezime
#define VALID_INPUT 1	// Konstanta koja označava valjan unos
#define INVALID_INPUT 0	// Konstanta koja označava nevaljan unos
#define MALLOC_ERROR -1	// Konstanta koja označava grešku pri alokaciji memorije koristeći malloc funkciju
#define EMPTY_LIST -2	// Konstanta koja označava praznu listu
#define FILE_ERROR_OPEN -3	// Konstanta koja označava grešku pri otvaranju datoteke
#define FILE_ERROR_WRITE -4	// Konstanta koja označava grešku pri pisanju u datoteku
#define SCANF_ERROR -5	// Konstanta koja označava grešku prilikom unosa koristeći scanf funkciju

#define RESET   "\033[0m"	// ANSI escape sekvenca za resetiranje svih stilova i boja na zadane postavke terminala
#define RED     "\033[31m"	// ANSI escape sekvenca za postavljanje boje teksta na crvenu
#define GREEN   "\033[32m"  // ANSI escape sekvenca za postavljanje boje teksta na zelenu
#define BOLD    "\033[1m"	// ANSI escape sekvenca za podebljavanje teksta (bold stil)

struct _Person;	// Unaprijedna deklaracija strukture _Person omogućava korištenje pokazivača na nju prije njene potpune definicije
typedef struct _Person* Position;	// Alias za pokazivač na strukturu _person, koristi se za povezivanje elemenata u listi
typedef struct _Person {	// Definicija strukture za osobu
	// Data
	char name[MAX_LENGTH];	// Polje za ime studenta (do 128 znakova)
	char surname[MAX_LENGTH];	// Polje za prezime studenta (do 128 znakova)
	int birthYear;	// Godina rođenja osobe
	// Link
	Position next;	// Pokazivač na sljedeću osobu u listi (za povezivanje elemenata u povezanoj listi)
} Person;	// Alias za strukturu _person, sada možemo koristiti 'person' kao tip podataka

// Prototipi funkcija koje će biti implementirane
int menu(Position head);	// Funkcija za prikaz glavnog izbornika s opcijama za korisnika.
int validStringInput(char* input, int max_length);	// Funkcija koja provjerava valjanost unosa stringa
int validBirthYear(int year);	// Funkcija koja provjerava valjanost unosa godine rođenja
Position createPerson();	// Funkcija koja stvara novu osobu u listi i vraća pokazivač na tu osobu
int prependList(Position head);	// Funkcija koja dodaje novu osobu na početak liste
int printPerson(Position person);	// Funkcija koja ispisuje podatke o jednoj osobi
int printList(Position first);	// Funkcija koja ispisuje trenutnu listu osoba
Position findLast(Position first);	// Funkcija koja pronalazi posljednju osobu u listi
int appendList(Position head);	// Funkcija koja dodaje novu osobu na kraj liste
Position findPersonBySurname(Position head, char* surname);	// Funkcija koja traži osobu u listi prema prezimenu i vraća pokazivač na tu osobu
Position findPreviousPerson(Position head, char* surname);	// Funkcija koja pronalazi prethodnu osobu u listi na temelju prezimena
int deletePerson(Position head, char* surname);	// Funkcija koja briše osobu iz liste prema prezimenu
int deleteList(Position head);	// Funkcija koja briše cijelu listu osoba i osigurava oslobađanje memorije
int insertAfterPerson(Position first, char* surname);	 // Funkcija koja ubacuje novu osobu nakon osobe s navedenim prezimenom u listi
int insertBeforePerson(Position head, char* surname);	// Funkcija koja ubacuje novu osobu prije osobe s navedenim prezimenom u listi
void sortListBySurname(Position head);	// Funkcija koja sortira listu osoba prema prezimenima po abecednom redosljedu
int writeListToFile(Position first);	// Funkcija koja zapisuje cijelu listu osoba u datoteku
int readNoRowsInFile();	// Funkcija koja vraća broj osoba (redaka u datoteci)
Position createPersonFromFile(char* name, char* surname, int birthYear);	// Funkcija koja stvara novi element (osobu) u listi na osnovu podataka pročitanih iz datoteke
Position findLastWithoutMessage(Position first);	// Funkcija koja pronalazi posljednju osobu U listi, bez poruke da je lista prazna jer će uvik bit na početku lista bit prazna
int addElementFromFile(Position head, char* name, char* surname, int birthYear);	// Funkcija koja dodaje osobu iz datoteke u listuB
int readListFromFile(Position head);	// Funkcija koja čita cijelu listu osoba iz datoteke






int main()
{
	Person head = { .name = {0}, .surname = {0}, .birthYear = 0, .next = NULL };	// Inicijalizacija čvora liste (head) s praznim podacima; ovo je početni čvor koji služi kao glava liste

	menu(&head);	// Pozivanje funkcije menu koja omogućuje korisniku upravljanje povezanim popisom osoba

	return EXIT_SUCCESS;	// Kraj programa, signalizira uspješan završetak
}

int menu(Position head)
{
	char choice = '\0';	// Varijabla za pohranu korisničkog izbora 
	char surname[MAX_LENGTH] = { 0 };	// Varijabla za pohranu prezimena unesena od strane korisnika
	Position found = NULL;	// Pokazivač za pohranu adrese pronađene osobe u listi

	do {
		// Ispis izbornika s dostupnim opcijama
		printf("\nChoose an option:\n");	// Ispis poruke koja traži od korisnika da odabere opciju
		printf("f - Add a new person at the front of the list\n");	// Dodavanje osobe na početak liste
		printf("p - Print the current list of people\n");	// Ispis svih osoba u trenutnoj listi
		printf("e - Add a new person at the end of the list\n");	// Dodavanje osobe na kraj liste
		printf("s - Find a person by surname in the list\n");	// Pronalaženje osobe prema prezimenu u listi
		printf("d - Delete a person by surname from the list\n");	// Brisanje osobe  iz liste prema prezimenu
		printf("a - Insert a person after the person with a given surname in the list\n");  // Ubacivanje nove osobe nakon osobe s navedenim prezimenom u listi
		printf("b - Insert a person before the person with a given surname in the list\n");  // Ubacivanje nove osobe prije osobe s navedenim prezimenom u listi
		printf("o - Sort the list of people by surname\n");  // Sortiranje liste osoba prema prezimenima
		printf("w - Write list to file\n");  // Zapisivanje liste osoba u datoteku
		printf("r - Read list from file\n");  // Čitanje liste osoba iz datoteke
		printf("x - Exit the program\n");	// Izlazak iz programa

		// Unos korisničkog izbora
		printf("Enter your choice: ");	// Ispis poruke koja traži od korisnika da unese svoj izbor
		if (scanf(" %c", &choice) != 1)	// Provjera je li unos valjan
		{
			printf(BOLD RED "Invalid input. Please enter a character.\n" RESET);	// Ispis poruke o nevažećem unosu u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			while (getchar() != '\n');	// Čisti ulazni spremnik (stdin) dok se ne dođe do kraja linije, čime se uklanjaju svi preostali znakovi u baferu
			continue;	// Nastavlja petlju dok se ne unese valjan unos
		}

		choice = tolower(choice);	// Pretvaranje korisničkog unosa u mala slova za jednostavniju usporedbu

		// Obrada korisničkog izbora pomoću switch naredbe
		switch (choice)	// Odabir korisničke opcije na temelju unesenog izbora
		{
		case 'f':	 // Ako korisnik odabere 'f', dodaje novu osobu na početak liste
			printf("Add a new person at the front of the list.\n");	// Ispis poruke da se dodaje nova osoba na početak liste
			prependList(head);	// Pozivanje funkcije za dodavanje nove osobe na početak liste
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'p':	// Ako korisnik odabere 'p', ispisuje trenutnu listu osoba
			printf("Printing the current list of persons...\n");	// Ispis poruke da se ispisuje trenutna list osoba
			printList(head->next);	// Pozivanje funkcije za ispis trenutne liste osoba
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'e':	// Ako korisnik odabere 'e', dodaje novu osobu na kraj liste
			printf("Add a new person at the end of the list.\n");	// Ispis poruke da se dodaje nova osoba na kraj liste
			appendList(head);	// Pozivanje funkcije za dodavanje nove osobe na kraj liste
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 's':	// Ako korisnik odabere 's', traži osobu u listi prema prezimenu
			printf("Find person with surname: ");	// Ispis poruke koja traži unos prezimena osobe koju korisnik želi pronaći
			while (1)	// Beskonačna petlja koja traje dok korisnik ne unese ispravan unos
			{
				if (scanf(" %127s", surname) != 1 || !validStringInput(surname, MAX_LENGTH))	// Provjerava je li ispravno uneseno prezime
				{
					printf(BOLD RED "Invalid input! Please enter a valid surname.\n" RESET);	// Ispis poruke o neispravnom unosu prezimena u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
					continue;  // Ako unos nije ispravan, ponovi unos
				}
				break;  // Ako je unos ispravan, izađi iz petlje
			}

			found = findPersonBySurname(head->next, surname);  // Poziv funkcije za pronalazak osobe u listi prema prezimenu
			if (found)	// Ako je osoba pronađena (found != NULL)
				printPerson(found);  // Pozivanje funkcije za ispis podataka pronađene osobe
			else	// Ako osoba nije pronađena	
				printf("Person with surname '%s' was not found.\n", surname);  // Ispis poruke o neuspješnom pronalasku tražene osobe
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'd':	// Ako korisnik odabere 'd', briše osobu iz liste prema prezimenu
			printf("Delete person with surname: ");	// Ispis poruke koja traži unos prezimena osobe koju korisnik želi obrisati
			while (1)	// Beskonačna petlja koja traje dok korisnik ne unese ispravan unos
			{
				if (scanf(" %127s", surname) != 1 || !validStringInput(surname, MAX_LENGTH))	// Provjerava je li ispravno uneseno prezime
				{
					printf(BOLD RED "Invalid input! Please enter a valid surname.\n" RESET);	// Ispis poruke o neispravnom unosu prezimena u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
					continue;  // Ako unos nije ispravan, ponovi unos
				}
				break;  // Ako je unos ispravan, izađi iz petlje
			}
			deletePerson(head, surname);	// Pozivanje funkcije za brisanje osobe iz liste prema prezimenu
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'a':	// Ako korisnik odabere 'a', ubacuje novu osobu nakon osobe s navedenim prezimenom u listi
			printf("Insert after person with surname: ");  // Ispis poruke koja traži unos prezimena osobe nakon koje korisnik želi ubaciti novu osobu
			while (1)	// Beskonačna petlja koja traje dok korisnik ne unese ispravan unos
			{
				if (scanf(" %127s", surname) != 1 || !validStringInput(surname, MAX_LENGTH))	// Provjerava je li ispravno uneseno prezime
				{
					printf(BOLD RED "Invalid input! Please enter a valid surname.\n" RESET);	// Ispis poruke o neispravnom unosu prezimena u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
					continue;  // Ako unos nije ispravan, ponovi unos
				}
				break;  // Ako je unos ispravan, izađi iz petlje
			}
			insertAfterPerson(head->next, surname);	// Pozivanje funkcije za ubacivanje nove osobe nakon osobe s navedenim prezimenom u listi
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'b':  // Ako korisnik odabere 'b', ubacuje novu osobu prije osobe s navedenim prezimenom u listi
			printf("Insert before person with surname: ");  // Ispis poruke koja traži unos prezimena osobe prije koje korisnik želi ubaciti novu osobu
			while (1)	// Beskonačna petlja koja traje dok korisnik ne unese ispravan unos
			{
				if (scanf(" %127s", surname) != 1 || !validStringInput(surname, MAX_LENGTH))	// Provjerava je li ispravno uneseno prezime
				{
					printf(BOLD RED "Invalid input! Please enter a valid surname.\n" RESET);	// Ispis poruke o neispravnom unosu prezimena u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
					continue;  // Ako unos nije ispravan, ponovi unos
				}
				break;  // Ako je unos ispravan, izađi iz petlje
			}
			insertBeforePerson(head, surname);	// Pozivanje funkcije za ubacivanje nove osobe prije osobe s navedenim prezimenom u listi
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'o':	// Ako korisnik odabere 'o', sortira listu osoba po prezimenima
			printf("Sorted list of people by surname in alphabetical order. ");	// Ispis poruke da je lista osoba sortirana po prezimenima u abecednom redoslijedu
			sortListBySurname(head);	// Pozivanje funkcije koja sortira listu osoba prema prezimenima u uzlaznom redoslijedu
			printList(head->next);	// Pozivanje funkcije za ispis trenutne liste osoba
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'w':	// Ako korisnik odabere 'w', zapisuje cijelu listu osoba u datoteku
			printf("Writing list to file...\n");	// Ispis poruke da se lista zapisuje u datoteku
			writeListToFile(head->next);	// Pozivanje funkcije za zapisivanje cijele liste osoba u datoteku
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'r':	// Ako korisnik odabere 'r', čita cijelu listu osoba iz datoteke
			printf("Reading list from file:\n");	// Ispis poruke da se lista čita iz datoteke
			deleteList(head);	//	Pozivanje funkcije koja oslobađa svu memoriju dodijeljenu za staru listu
			readListFromFile(head);	// Pozivanje funkcije za čitanje cijele liste osoba iz datoteke
			printList(head->next);
			break;	// Izlazi iz switch bloka nakon što je opcija obrađena

		case 'x':	// Ako korisnik odabere 'x', izlazi iz programa
			printf("Exiting the program.\n");	// Ispis poruke o izlasku iz programa
			deleteList(head);	//	Pozivanje funkcije koja oslobađa svu memoriju dodijeljenu za listu
			return EXIT_SUCCESS;	// Kraj programa, signalizira uspješan završetak

		default:	// Ako korisnik unese bilo koju drugu opciju koja nije među ponuđenim
			printf(BOLD RED "Invalid choice. Try again.\n"  RESET);	// Ispis poruke o nevažećem odabiru u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			break;	// Izlazi iz switch bloka i nastavlja izvođenje ostatka petlje
		}

	} while (choice != 'x');	 // Petlja traje sve dok korisnik ne unese 'x' za izlaz

	deleteList(head);	//	Pozivanje funkcije koja oslobađa svu memoriju dodijeljenu za listu

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int validStringInput(char* input, int max_length)
{
	int i = 0;	// Indeks za iteraciju kroz znakove unosa

	if (strlen(input) == 0)	// Provjera je li unos prazan
	{
		printf(BOLD RED "Input cannot be empty!\n" RESET);	// Ispis poruke o praznom unosu u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
		return INVALID_INPUT; // Povratna vrijednost označava nevaljan unos
	}

	for (i = 0; i < strlen(input); i++)	// Provjera sadrži li unos samo slova
	{
		if (!isalpha(input[i]))	// Provjera je li trenutni znak abecedni znak (slovo)
		{
			printf(BOLD RED "Input can only contain alphabetic characters!\n" RESET);	// Ispis poruke ako unos sadrži nevažeće znakove u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			return INVALID_INPUT; // Povratna vrijednost označava nevaljan unos
		}
	}

	if (strlen(input) >= max_length)	// Provjera dužine unosa
	{
		printf(BOLD RED "Input exceeds maximum length of %d characters!\n" RESET, max_length);	// Ispis poruke ako unos premašuje maksimalnu duljinu u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
		return INVALID_INPUT; // Povratna vrijednost označava nevaljan unos
	}

	return VALID_INPUT; // Povratna vrijednost označava valjan unos
}

int validBirthYear(int year)
{
	int currentYear = 2025; // Varijabla koja definira trenutnu godinu

	if (year < 1900 || year > currentYear)	 // Provjera je li godina manja od 1900 ili veća od trenutne godine 
	{
		printf(BOLD RED "Invalid year! Year must be between 1900 and %d.\n" RESET, currentYear);	// Ispis poruke o nevažećoj godini u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
		return INVALID_INPUT; // Povratna vrijednost označava nevaljan unos
	}

	return VALID_INPUT; // Povratna vrijednost označava valjan unos
}


Position createPerson()
{
	Position newPerson = NULL;	// Pokazivač na dinamički alocirani novi element (osobu) koji će biti ubačen u listu tipa Position
	char name[MAX_LENGTH] = { 0 };	// Polje za pohranu imena osobe
	char surname[MAX_LENGTH] = { 0 };	// Polje za pohranu prezimena osobe
	int birthYear = 0;	// Varijabla za pohranu godine rođenja osobe
	int validName = 0;	// Varijabla koja označava valjanost unosa imena, inicijalizirana na 0 što označava da je unos imena nevaljan
	int validSurname = 0;	// Varijabla koja označava valjanost unosa prezimena, inicijalizirana na 0 što označava da je unos prezimena nevaljan
	int validYear = 0;	// Varijabla koja označava valjanost unosa godine, inicijalizirana na 0 što označava da je unos godine nevaljan

	newPerson = (Position)malloc(sizeof(Person));	// Dinamičko alociranje memorije za novi element (osobu)  [newPerson =(Person*)malloc(sizeof(Person));]
	if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		return NULL;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije, NULL a ne MALLOC_ERROR jer funckija vraća pokazivač
	}

	while (!validName)	// Petlja koja traje dok korisnik ne unese ispravno ime (ili validName == 0)
	{
		printf("Enter name:\n");	// Ispis poruke koja traži od korisnika da unese ime osobe 
		if (scanf(" %127s", name) != 1 || !validStringInput(name, MAX_LENGTH))	// Provjera je li unos imena uspješan
		{
			printf(BOLD RED "Invalid input!\n" RESET);	// Ispis poruke o grešci ako čitanje podatka o imenu nije uspješno u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			continue;	// Nastavlja petlju dok se ne unese valjan unos
		}
		validName = 1;	// Postavljanje varijable validName na 1, što označava da je unos imena valjan
	}

	while (!validSurname)	// Petlja koja traje dok korisnik ne unese ispravno prezime (ili validSurname == 0)
	{
		printf("Enter surname:\n");	// Ispis poruke koja traži od korisnika da unese prezime osobe
		if (scanf(" %127s", surname) != 1 || !validStringInput(surname, MAX_LENGTH))	// Provjera je li unos prezimena uspješan
		{
			printf(BOLD RED "Invalid input!\n" RESET);	// Ispis poruke o grešci ako čitanje podatka o prezimenu nije uspješno u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			continue;	// Nastavlja petlju dok se ne unese valjan unos
		}
		validSurname = 1;	// Postavljanje varijable validSurname na 1, što označava da je unos prezimena valjan
	}

	while (!validYear)	// Petlja koja traje dok korisnik ne unese ispravnu godinu (ili validYear == 0)
	{
		printf("Enter birth year:\n");	// Ispis poruke koja traži od korisnika da unese godinu rođenja osobe
		if (scanf(" %d", &birthYear) != 1 || !validBirthYear(birthYear))	// Provjera je li unos godine rođenja uspješan
		{
			printf(BOLD RED "Invalid input!\n" RESET);	// Ispis poruke o grešci ako čitanje podatka o godinu rođenja nije uspješno u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
			continue;	// Nastavlja petlju dok se ne unese valjan unos
		}
		validYear = 1;	// Postavljanje varijable validYear na 1, što označava da je unos godine valjan
	}

	// Kopiranje unesenih podataka u strukturu
	strcpy(newPerson->name, name);	// Kopiranje unesenog imena u odgovarajuće polje strukture
	strcpy(newPerson->surname, surname);	// Kopiranje unesenog prezimena u odgovarajuće polje strukture
	newPerson->birthYear = birthYear;	// Postavljanje unesenih godina rođenja u strukturu
	newPerson->next = NULL;	// Inicijalizacija pokazivača 'next' na NULL, označava kraj liste

	return newPerson;	// Vraća pokazivač na novokreirani element (osobu)
}


int prependList(Position head)
{
	Position newPerson = NULL;	// Pokazivač na novi element (osobu) koji će biti ubačen u listu  

	newPerson = createPerson();	// Pozivanje funkcije koja stvara novi element (osobu) i vraća njen pokazivač
	if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije
	}

	// Dodavanje nove osobe na početak liste
	newPerson->next = head->next;	// Postavljanje pokazivača 'next' novog elementa (osobe) na trenuti prvi član liste	
	head->next = newPerson;	// Ažuriranje pokazivača na početak liste, novi element (osoba) postaje prvi član liste


	return EXIT_SUCCESS;	// Kraj programa, signalizira uspješan završetak
}

int printPerson(Position person)
{
	printf(BOLD GREEN "Name: %-10s  Surname: %-15s  Birth year: %d\n" RESET, person->name, person->surname, person->birthYear);	// Ispis podataka za trenutni element (osobu) liste u zelenoj boji s podebljanim stilom 
	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int printList(Position first)
{
	Position current = first;	// Pokazivač 'current' (trenutnog elementa (osobe)), inicijaliziran na prvi element (osobu) liste

	if (!current)	// Provjera je li lista prazna (prvi element je NULL) (ili current == NULL)
	{
		printf("Empty list!\n");	// Ispis poruku ako je lista prazna
		return EMPTY_LIST;	 // Povratna vrijednost koja označava praznu listu
	}

	printf("Current list of persons:\n");	// Ispis poruke koja označava početak ispisa liste osoba
	while (current)	// Prolazak kroz listu dok ne dođemo do kraja (ili current != NULL)
	{
		printPerson(current);	// Pozivanje funkcije za ispis podataka o trenutnoj osobi u listi
		current = current->next;	// Pomicanje na sljedeći element (osobu) u listi
	}

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

Position findLast(Position first)
{
	Position current = first;	// Pokazivač 'current' na trenutni element (osobu), inicijaliziran na prvi element (osobu) u listi

	if (!current)	// Provjera je li lista prazna (prvi element je NULL) (ili current == NULL)
	{
		printf("Empty list!\n");	// Ispisuje poruku ako je lista prazna
		return NULL;	// Povratna vrijednost koja označava praznu listu, NULL a ne EMPTY_LIST jer funckija vraća pokazivač
	}

	while (current->next != NULL)	// Prolazak kroz listu dok ne dođemo do posljednjeg elementa (ili current->next)
		current = current->next;	// Pomicanje na sljedeći element (osobu) u listi

	return current;	// Vraća pokazivač na posljednji element (osobu)
}

int appendList(Position head)
{
	Position newPerson = NULL;	// Pokazivač na novi element (osobu) koji će biti ubačen u listu	
	Position last = NULL;;	// Pokazivača na zadnji element (osobu) liste 

	newPerson = createPerson();	// Pozivanje funkcije koja stvara novi element (osobu) i vraća njen pokazivač
	if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije
	}

	last = findLast(head->next);	// Pozivanje funkcije da pronađemo posljednji element (osobu) u listi

	if (!last)	// Ako je lista prazna (head->next == NULL)
		head->next = newPerson;	// Postavljanje novog elementa (osobe) kao prvi član liste

	else		// Ako lista nije prazna 
		// newPerson->next = last->next; Postavljanje next pokazivača novog elementa na NULL (kraj liste), suvišno jer je last->next uvijek NULL a newPerson->next = NULL je definirano u createPerson()
		last->next = newPerson;	// Postavljanje novog elementa (osobe) kao posljednjeg člana liste

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}


Position findPersonBySurname(Position first, char* surname)
{
	Position current = first;	// Pokazivač 'current' na trenutni element (osobu), inicijaliziran na prvi element (osobu) u listi

	while (current != NULL && strcmp(current->surname, surname) != 0)	 // Prolazak kroz listu dok ne dođe do podudaranja prezimena ili dok ne dođe do kraja liste
		current = current->next;	// Pomicanje na sljedeći element (osobu) u listi

	return current;  // Vraća pokazivač na pronađeni element (osobu) ili NULL ako nije pronađena ili ako je list prazna
}


Position findPreviousPerson(Position head, char* surname)
{
	Position previous = head;	// Pokazivač 'previous' na prethodni element (osobu), inicijaliziran na početak liste (head)

	while (previous->next != NULL && strcmp(previous->next->surname, surname) != 0)	// Prolazak kroz listu dok ne dođe do podudaranja prezimena ili dok ne dođe do kraja liste
		previous = previous->next;	// Pomicanje na sljedeći element liste

	if (previous->next != NULL)	// Ako sljedeći element (osoba) od 'previous' (prethnodnog elementa (osobe)) postoji tj. nije NULL (ili previous->next)
		return	previous;	// Vraća pokazivač na taj prethodni element (osobu)
	else	// Ako ne postoji sljedeći element (osoba) od 'previous' (prethnodnog elementa (osobe))
		return NULL;	// Povratna vrijednost koja označava ili da element (osoba) nije pronađena ili da je lista prazna
}


int deletePerson(Position head, char* surname)
{
	Position previous = head;	// Pokazivač 'previous' na prethodni element (osobu), inicijaliziran na početak liste (head)
	Position temp = NULL;	// Pokazivač za privremeno spremanje elementa (osobe) koji se briše iz liste

	previous = findPreviousPerson(head, surname);	// Pozivanje funkcije za pronalaženje prethodnog elementa (osobe) s traženim prezimenom

	if (previous)	// Ako element (osoba) traženog prezimena pronađen (ili previous != NULL)
	{
		temp = previous->next;	// Spremanje trenutnog elementa (osobe) (kojeg brišemo) u privremenu varijablu
		previous->next = previous->next->next;	// Preskakanje trenutnog elementa (osobe) u listi (spajanje prethodnog i sljedećeg elementa (osobe))
		free(temp);	// Oslobađanje memorije za element (osobu) koji brišemo
		printf("Person with surname '%s' successfully deleted.\n", surname);	 // Ispis poruke da je osoba uspješno obrisana
	}
	else	// Ako element (osoba) traženog prezimena nije pronađen
		printf("Person with surname '%s' was not found.\n", surname);	 // Ispis poruke da osoba s tim prezimenom nije nađena

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}


int deleteList(Position head)
{
	Position temp = NULL;	// Pokazivač za privremeno spremanje elementa (osobe) koji se briše iz liste

	while (head->next != NULL) // Prolazak kroz listu dok god postoji sljedeći element (osoba) (ili head->next)
	{
		temp = head->next;	// Spremanje sljedećeg elementa (osobe) u privremenu varijablu
		head->next = head->next->next;	// Preskakanje trenutnog elementa (osobe) u listi (spajanje prethodnog i sljedećeg elementa (osobe))
		free(temp);	// Oslobađanje memorije za element (osobu) koji brišemo
	}

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int insertAfterPerson(Position first, char* surname)
{
	Position current = first;	// Pokazivač 'current' na trenutni element (osobu), inicijaliziran na prvi element (osobu) u listi
	Position newPerson = NULL;	// Pokazivač na novi element (osobu) koji će biti ubačen u listu

	current = findPersonBySurname(first, surname);	// Traži osobu s navedenim prezimenom u listi

	if (current)	// Ako element (osoba) traženog prezimena pronađen (ili current != NULL)
	{
		newPerson = createPerson();	// Pozivanje funkcije koja stvara novi element (osobu) i vraća njen pokazivač
		if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
		{
			printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
			return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije
		}

		newPerson->next = current->next;	// Novi element (osoba) sada pokazuje na trenutni (navedeni) element (osobu) 
		current->next = newPerson;	// Trenutni (navedeni) element (osoba) sada pokazuje na novi element (osobu)
	}	
	else	// Ako element (osoba) traženog prezimena nije pronađen
		printf("Person with surname '%s' was not found.\n", surname);	// Ispis poruke da osoba s tim prezimenom nije nađena
	
	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int insertBeforePerson(Position head, char* surname)
{
	Position previous = head;	// Pokazivač 'previous' na prethodni element (osobu), inicijaliziran na početak liste (head)
	Position newPerson = NULL;	// Pokazivač na novi element (osobu) koji će biti ubačen u listu

	previous = findPreviousPerson(head, surname);

	if (previous)	// Ako element (osoba) traženog prezimena pronađen (ili previous != NULL)
	{
		newPerson = createPerson();	// Pozivanje funkcije koja stvara novi element (osobu) i vraća njen pokazivač
		if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
		{
			printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
			return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije
		}

		newPerson->next = previous->next;	// Novi element (osoba) sada pokazuje na sljedeći element (osobu) od 'previous', navedeni element (osoba) od korisnika prema prezimenu
		previous->next = newPerson;	// Prethodni element (osoba) od navedenog elementa (osobe) sada pokazuje na novi element (osobu)
	}
	else	// Ako element (osoba) traženog prezimena nije pronađen
		printf("Person with surname '%s' was not found.\n", surname);	// Ispis poruke da osoba s tim prezimenom nije nađena

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

void sortListBySurname(Position head)
{
	Position previous = NULL;	// Pokazivač koji čuva prethodni element (osobu) u listi
	Position current = NULL;	// Pokazivač na trenutni element (osobu) u listi
	Position end = NULL;	// Pokazivač na kraj liste, koristi se za označavanje kraja nepotrebnog dijela
	Position temp = NULL;	// Pokazivač za privremeno spremanje elementa (osobe) pri zamjeni elemenata (osoba)

	while (head->next != end)	 // Vanjska petlja koja prolazi kroz listu dok ne dođe do kraja
	{
		previous = head;	// Postavljanje prethodnog elementa (osobe) na početak liste (head)
		current = head->next;	// Postavljanje trenutnog elementa (osobe) na prvi element (osobu) u listi 
		while (current->next != end)	// Unutarnja petlja koja prolazi kroz listu od trenutnog elementa (osobe) do kraja
		{
			if (strcmp(current->surname, current->next->surname) > 0)	// Provjera je li prezime trenutnog elementa (osobe) veće od prezimena sljedećeg lementa (osobe), ako je prvo prezime abecedno isprid od drugog prezimena strcmp vraća pozitivnu vrijednost
			{
				// Zamjena položaja trenutnog i sljedećeg elementa (osobe)
				temp = current->next;	// Privremeno spremanje sljedećeg elementa (osobe)
				previous->next = temp;	// Prethodni element (osoba) sada pokazuje na sljedeći element (osobu)
				current->next = temp->next;	// Trenutni element (osoba) sada pokazuje na element (osobu) nakon sljedećeg
				temp->next = current;	// Sljedeći element (osoba) sada pokazuje na trenutni element (osobu)

				current = temp;	// Premještanje trenutnog pokazivača na zamijenjeni element (osobu)
			}
			previous = current;	// Premještanje prethodnog pokazivača na trenutni element (osobu)
			current = current->next;	// Premještanje trenutnog pokazivača na sljedeći element (osobu)
		}
		end = current;	// Postavljanje kraja na trenutni element (osobu), kako bi sljedeća iteracija (prolaz kroz petlju) radila samo sa preostalim neurednim elementima (osobama)
	}
}

int writeListToFile(Position first)
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	Position current = first;	// Pokazivač 'current' na trenutni element (osobu), inicijaliziran na prvi element (osobu) u listi

	filePointer = fopen("persons.txt", "w");	// Otvaranje datoteke "persons.txt" u načinu za pisanje ("w")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n");	// Ispis poruke o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;	// Povratna vrijednost označava grešku pri otvaranju datoteke
	}

	if (!current)	// Provjera je li lista prazna (prvi element je NULL) (ili current == NULL)
	{
		printf("Empty list!\n");	// Ispis poruku ako je lista prazna
		fclose(filePointer);  // Zatvaranje datoteke
		return EMPTY_LIST;	// Povratna vrijednost koja označava praznu listu
	}

	fprintf(filePointer, "%-10s %-15s %-10s\n", "NAME", "SURNAME", "BIRTH YEAR");	// Ispis zaglavlja u datoteku

	while (current)	// Prolazak kroz listu sve dok ne dođemo do kraja (ili current != NULL)
	{
		if (fprintf(filePointer, "%-10s %-15s %d\n", current->name, current->surname, current->birthYear) < 0)	// Provjerava jesu li uspješno zapisane vrijednosti (ime, prezime i bodove) iz liste u datoteku, ako nije uspješno funkcija fprintf() vraća negativnu vrijednost
		{
			printf("Error writing list to file!\n");	// Ispis poruke o grešci ako zapis podataka iz liste u datoteku nije uspješan
			fclose(filePointer);  // Zatvaranje datoteke
			return FILE_ERROR_WRITE;	// Povratna vrijednost koja označava grešku pri zapisivanju liste u datoteku
		}
		current = current->next;	// Pomicanje na sljedeći element (osobu) u listi
	}

	fclose(filePointer);	// Zatvaranje datoteke

	printf(BOLD GREEN "List successfully written to file!\n" RESET);	// Ispis poruke o uspješnom zapisivanju liste u datoteku u zelenoj boji s podebljanim stilom 

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int readNoRowsInFile()
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	char buffer[MAX_LINE] = { 0 };	// Buffer za privremeno pohranjivanje pročitanog reda
	int rowCounter = 0;	// Brojač redaka u datoteci

	filePointer = fopen("persons.txt", "r");	// Otvaranje datoteke "persons.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("Error opening file!\n");	// Ispis poruke o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;	// Povratna vrijednost koja označava grešku pri otvaranju datoteke
	}

	fgets(buffer, MAX_LINE, filePointer);	// Preskakanje prvog reda (zaglavlja)

	while (fgets(buffer, MAX_LINE, filePointer) != NULL)	// Čita jedan redak iz datoteke i pohranjuje ga u buffer dok ne dođe do kraja datoteke (NULL označava kraj datoteke)
		rowCounter++;	// Povećava brojač redaka nakon svakog uspješno pročitanog reda

	fclose(filePointer);	// Zatvaranje datoteke

	return rowCounter;	// Vraća broj redaka u datoteci (broj osoba)
}

Position createPersonFromFile(char* name, char* surname, int birthYear)
{
	Position newPerson = NULL;	// Pokazivač na dinamički alocirani novi element (osobu) koji će biti ubačen u listu tipa Position
	int currentYear = 2025;	// Varijabla koja definira trenutnu godinu

	newPerson = (Position)malloc(sizeof(Person));	// Dinamičko alociranje memorije za novi element (osobu)  [newPerson =(Person*)malloc(sizeof(Person));]
	if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		return NULL;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije, NULL a ne MALLOC_ERROR jer funckija vraća pokazivač
	}

	if (strlen(name) >= MAX_LENGTH || strlen(surname) >= MAX_LENGTH || birthYear < 1900 || birthYear > currentYear)	// Provjera duljine imena / prezimena te ispravnosti godine rođenja(1900 ≤ birthYear ≤ currentYear)
	{
		printf(BOLD RED "Invalid input!\n" RESET);	// Ispis poruke o grešci ako učitani podaci nisu valjani u crvenoj boji s podebljanim stilom kako bi korisnik primijetio grešku
		free(newPerson);	// Oslobađanje prethodne alocirane memorije
		return NULL;	// Povratna vrijednost označava nevaljan unos, NULL a ne INVALID_INPUT jer funckija vraća pokazivač
	}

	// Kopiranje unesenih podataka u strukturu
	strcpy(newPerson->name, name);	// Kopiranje unesenog imena u odgovarajuće polje strukture
	strcpy(newPerson->surname, surname);	// Kopiranje unesenog prezimena u odgovarajuće polje strukture
	newPerson->birthYear = birthYear;	// Postavljanje unesenih godina rođenja u strukturu
	newPerson->next = NULL;	// Inicijalizacija pokazivača 'next' na NULL, označava kraj liste


	return newPerson;	// Vraća pokazivač na novokreirani element (osobu)
}

Position findLastWithoutMessage(Position first)
{
	Position current = first;	// Pokazivač 'current' na trenutni element (osobu), inicijaliziran na prvi element (osobu) u listi

	if (!current)	// Provjera je li lista prazna (prvi element je NULL) (ili current == NULL)
		return NULL;	// Povratna vrijednost koja označava praznu listu, NULL a ne EMPTY_LIST jer funckija vraća pokazivač

	while (current->next != NULL)	// Prolazak kroz listu dok ne dođemo do posljednjeg elementa (ili current->next)
		current = current->next;	// Pomicanje na sljedeći element (osobu) u listi

	return current;	// Vraća pokazivač na posljednji element (osobu)
}

int addElementFromFile(Position head, char* name, char* surname, int birthYear)
{
	Position newPerson = NULL;	// Pokazivač na novi element (osobu) koji će biti ubačen u listu  
	Position last = NULL;	// Pokazivača na zadnji element (osobu) liste


	newPerson = createPersonFromFile(name, surname, birthYear);	// Pozivanje funkcije koja stvara novi element (osobu) na osnovu podataka pročitanih iz datoteke i vraća njen pokazivač
	if (!newPerson)	// Provjera je li alokacija memorije uspješna (ili newPerson == NULL)
	{
		printf("Error allocating memory!\n");	// Ispis poruke o grešci ako alokacija memorije nije uspjela
		return MALLOC_ERROR;	// Povratna vrijednost koja označava grešku pri alokaciji memorije pomoću malloc funkcije
	}

	last = findLastWithoutMessage(head->next);	// Pozivanje funkcije da pronađemo posljednji element (osobu) u listi
	if (!last)	// Ako je lista prazna (head->next == NULL)
		head->next = newPerson;	// Postavljanje novog elementa (osobe) kao prvi član liste

	else		// Ako lista nije prazna 
		last->next = newPerson;	// Postavljanje novog elementa (osobe) kao posljednjeg člana liste

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}

int readListFromFile(Position head)
{
	FILE* filePointer = NULL;	// Pokazivač na datoteku
	int noRows = 0;	// Varijabla za broj redaka (osoba) u datoteci 
	char buffer[MAX_LENGTH] = { 0 };	// Buffer za privremeno pohranjivanje pročitanog reda
	int i = 0;	// Indeks za iteraciju kroz redove datoteke 
	char name[MAX_LENGTH] = { 0 };	// Polje za privremeno pohranjivanje imena osobe
	char surname[MAX_LENGTH] = { 0 };	// Polje za privremeno pohranjivanje prezimena osobe
	int birthYear = 0;	// Varijabla za privremeno pohranjivanje godine rođenja osobe
	
	

	filePointer = fopen("persons.txt", "r");	// Otvaranje datoteke "persons.txt" u načinu za čitanje ("r")
	if (!filePointer)	// Provjera je li datoteka uspješno otvorena (ili filePointer == NULL)
	{
		printf("File could not be opened.\n");	// Ispis poruke o grešci ako datoteka nije otvorena
		return FILE_ERROR_OPEN;	// Povratna vrijednost koja označava grešku pri otvaranju datoteke
	}

	noRows = readNoRowsInFile();	// Pozivanje funkcije koja vraća broj osoba iz datoteke

	fgets(buffer, MAX_LENGTH, filePointer);	// Preskakanje prvog reda (zaglavlja)

	if (noRows > 0)	// Ako je broj redaka (osoba) veći od 0
	{
		for (i = 0; i < noRows; i++)	// Iteracija kroz sve redove (osobe)u datoteci
		{
			
			if (fscanf(filePointer, "%s %s %d", name, surname, &birthYear) != 3)	// Provjerava jesu li uspješno pročitane 3 vrijednosti (ime, prezime i bodove) iz datoteke koje se spremaju u trenutnu strukturu 'Person'
			{
				printf("Error reading list from file!\n");	// Ispis poruke o grešci ako čitanje liste iz datoteke nije uspješan
				fclose(filePointer);	// Zatvaranje datoteke
				return SCANF_ERROR;	// Povratna vrijednost koja označava grešku pri čitanju liste iz datoteke
			}

			addElementFromFile(head, name, surname, birthYear);	// Pozivanje funkcije za dodavanje novog elementa (osobe) u listu na temelju pročitanih podataka
			
		}
	}
	else	// Ako datoteka ne sadrži redove podataka
		printf("No rows to read from the file.\n");	// Ispis poruke da nema redaka u datoteci za čitanje
		

	fclose(filePointer);	// Zatvaranje datoteke

	printf(BOLD GREEN "List successfully read from file!\n" RESET);	// Ispis poruke o uspješnom čitanju liste iz datoteku u zelenoj boji s podebljanim stilom 

	return EXIT_SUCCESS;	// Kraj funkcije, signalizira uspješan završetak
}





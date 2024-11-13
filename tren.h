#ifndef TREN_H_
#define TREN_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Vagon
{
    char nume[1]; // numele vagonului
	struct Vagon *next; // link catre urmatorul vagon
	struct Vagon *prev; // link catre vagonul precedent
} Vagon;

typedef struct Tren {
    struct Vagon *locomotiva; // adresa unde se afla locomotiva (santinela)
    struct Vagon *mecanic; // adresa vagonului la care se afla mecanicul
} Tren;

void MOVE_LEFT(Tren *tren) {
    if (tren->mecanic->prev == tren->locomotiva) // daca mecanicul se afla in primul vagon
        tren->mecanic = tren->locomotiva->prev;
    else
        tren->mecanic = tren->mecanic->prev;
}

void MOVE_RIGHT(Tren *tren) {
    if (tren->mecanic->next == tren->locomotiva) { // cazul in care mecanicul se afla in ultimul vagon
        Vagon *nou = (Vagon*)malloc(sizeof(struct Vagon));
        nou->nume[0] = '#'; // creez un nou vagon cu numele "#"
        nou->next = tren->locomotiva; // refac legaturile si conectez noul nod
        tren->mecanic->next = nou; // la locomotiva si la ultimul vagon
        nou->prev = tren->mecanic;
        tren->locomotiva->prev = nou;
        tren->mecanic = tren->mecanic->next;
    }
    else
        tren->mecanic = tren->mecanic->next;
}

void WRITE(Tren *tren, char *inscriptie) {
    strcpy(tren->mecanic->nume, inscriptie);
}

void CLEAR_CELL(Tren *tren) {
    if (tren->mecanic->next == tren->locomotiva && tren->mecanic->prev == tren->locomotiva) // cazul pentru un singur vagon
        WRITE(tren, "#");
    else {
        tren->mecanic->prev->next = tren->mecanic->next; // refac legaturile
        tren->mecanic->next->prev = tren->mecanic->prev;
        MOVE_LEFT(tren);
    }
}

void CLEAR_ALL(Tren *tren) {
    tren->mecanic = tren->locomotiva->prev; // pornim cu mecanicul de la ultimul vagon
    while (tren->mecanic->prev != tren->locomotiva) // eliminam cate un vagon pana cand mecanicul ajunge in primul vagon
        CLEAR_CELL(tren);
    WRITE(tren, "#"); // ultimul vagon il denumesc "#"
}

void INSERT_LEFT(Tren *tren, char *inscriptie, FILE *output) {
    if (tren->mecanic->prev == tren->locomotiva) // cazul cand mecanicul e in primul vagon
        fprintf(output, "%s\n", "ERROR");
    else {
        Vagon *nou = (Vagon*)malloc(sizeof(struct Vagon));
        strcpy(nou->nume, inscriptie); // creez un nou vagon cu numele precizat la apelul functiei
        tren->mecanic->prev->next = nou; // refac legaturile
        nou->next = tren->mecanic;
        nou->prev = tren->mecanic->prev;
        tren->mecanic->prev = nou;
        tren->mecanic = nou; // mut mecanicul in noul vagon
    }
}

void INSERT_RIGHT(Tren *tren, char *inscriptie) {
    Vagon *nou = (Vagon*)malloc(sizeof(struct Vagon));
    strcpy(nou->nume, inscriptie); // creez un nou vagon cu numele precizat la apelul functiei
    nou->next = tren->mecanic->next; // refac legaturile
    nou->prev = tren->mecanic;
    tren->mecanic->next->prev = nou;
    tren->mecanic->next = nou;
    tren->mecanic = nou; // mut mecanicul in noul vagon
}

void SEARCH(Tren *tren, char *S, FILE *output) {
    int i = 0, nr = 0;
    Vagon *pos, *temp;
    pos = tren->mecanic;
    temp = tren->mecanic;
    do { // parcurg pana ajung din nou in pozitia initala a mecanicului
        if (temp->nume[0] == S[i]) { // verific daca numele vagonului curent corespunde cu litera curenta a sirului
            nr++; // daca da, contorizez acest lucru
            if (nr == 1)
                pos = temp;
            i++;
        }
        else {
            nr = 0;
            i = 0;
        }
        if (temp->next == tren->locomotiva)
            temp = tren->locomotiva->next;
        else
            temp = temp->next; // mut mecanicul in urmatorul vagon
    } while (temp != tren->mecanic && i < strlen(S));
    if (nr == strlen(S)) // daca lungimea lui S este egala cu nr inseamna ca s-a gasit sirul
        tren->mecanic = pos; // deplasez mecanicul la pozitia unde incepe sirul
    else
        fprintf(output, "%s\n", "ERROR");
}

void SEARCH_RIGHT(Tren *tren, char *S, FILE *output) {
    int i = 0, nr = 0;
    Vagon *pos, *temp;
    pos = tren->mecanic;
    temp = tren->mecanic;
    while (temp != tren->locomotiva && i < strlen(S)) { // parcurg pana la ultimul vagon 
        if (temp->nume[0] == S[i]) { // verific daca numele vagonului curent corespunde cu caracterul curent al sirului
            nr++; // daca da, contorizez acest lucru
            pos = temp; // saalvez pozitia vagonului ce contine ultimul caracter
            i++;
        }
        else { // daca caracterul nu corespunde, resetez cautarea
            nr = 0;
            i = 0;
        }
        temp = temp->next; // mut mecanicul in urmatorul vagon
    }
    if (nr == strlen(S))  // daca lungimea lui S este egala cu nr inseamna ca s-a gasit sirul
        tren->mecanic = pos;  // deplasez mecanicul la pozitia unde se termina sirul
    else
        fprintf(output, "%s\n", "ERROR");
}

void SEARCH_LEFT(Tren *tren, char *S, FILE *output) {
    int i, nr = 0;
    Vagon *pos, *temp;
    i = 0;
    pos = tren->mecanic;
    temp = tren->mecanic;
    while (temp != tren->locomotiva && i < strlen(S)) { // parcurg pana la primul vagon 
        if (temp->nume[0] == S[i]) { // verific daca numele vagonului curent corespunde cu caracterul curent al sirului
            nr++; // daca da, contorizez acest lucru
            pos = temp;
            i++;
        }
        else { // daca caracterul nu corespunde, resetez cautarea
            nr = 0;
            i = 0;
        }
        temp = temp->prev; // mut mecanicul in vagonul din fata
    }
    if (nr == strlen(S))  // daca lungimea lui S este egala cu nr inseamna ca s-a gasit sirul
        tren->mecanic = pos;  // deplasez mecanicul la pozitia unde se termina sirul
    else
        fprintf(output, "%s\n", "ERROR");
}

void SHOW_CURRENT(Tren *tren, FILE *output) {
    fprintf(output, "%s\n", tren->mecanic->nume);
}

void SHOW(Tren *tren, FILE *output) {
    Vagon *vagon = tren->locomotiva->next; // variabila cu care parcurg trenul, o initializez cu primul vagon
    while (vagon != tren->locomotiva) {
        if (vagon == tren->mecanic)
            fprintf(output, "|%s|", vagon->nume); // afisez vagonul in care se afla mecanicul
        else
            fprintf(output, "%s", vagon->nume); // celelalte vagoane
        vagon = vagon->next;
    }
    fprintf(output, "\n");
}

#endif
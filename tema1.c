#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tren.h"

typedef struct QueueNode{
    char comanda[20];
	struct QueueNode *next;
}QueueNode;

typedef struct Queue{
    QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

Queue* createQueue(void) {
	// creeaza o coada goala
	Queue* queue = (Queue*)malloc(sizeof(struct Queue));
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;
    return queue;
}

int isQueueEmpty(Queue *q) {
	// verifica daca coada este goala
	if (q->front == NULL && q->rear == NULL && q->size == 0)
	    return 1;
    return 0;
}

void enqueue(Queue *q, char *comanda) {
	// adauga un element la finalul cozii
	QueueNode *node = (QueueNode*)malloc(sizeof(struct QueueNode));
	strcpy(node->comanda, comanda);
	node->next = NULL;
	if (isQueueEmpty(q)) {
		q->front = node;
		q->rear = node;
	}
    else {
	q->rear->next = node;
    q->rear = node;
	}
	q->size++;
}

void enqueue2(Queue *q, char *comanda) {
	// adauga un element la inceputul cozii si o folosesc pentru SWITCH
	QueueNode *node = (QueueNode*)malloc(sizeof(struct QueueNode));
	strcpy(node->comanda, comanda);
	node->next = NULL;
	if (isQueueEmpty(q)) { // cand coada este goala
		q->front = node;
		q->rear = node;
	}
    else {
	node->next = q->front; // leg noul nod de varful anterior al cozii
	q->front = node; // actualizez varful cozii
	}
	q->size++;
}

void dequeue(Queue* q) {
	// elimina primul element din coada
	QueueNode *temp;
	if (!isQueueEmpty(q)) {
	    temp = q->front;
		q->front = q->front->next;
		free(temp);
		q->size--;
    	if (q->front == NULL)
		q->rear = NULL;
	}
}

void dequeue2(Queue* q) {
	// elimina ultimul element din coada
    if (q->size == 1) { // cazul cand coada are un singur element si nu exista temp->next->next
        free(q->rear);
        q->front = NULL;
        q->rear = NULL;
        q->size = 0;
        return;
    }
	QueueNode *temp = q->front;
    while (temp->next->next != NULL) // parcurg coada pana la penultimul element
        temp = temp->next;
    free(temp->next); // dau free la ultimul element
    temp->next = NULL; // leg nodul la NULL
	q->size--;
    q->rear = temp; // actualizez ultimul element din coada
}

void destroyQueue(Queue *q){
	// distruge coada
    while (!isQueueEmpty(q))
	    dequeue(q);
    free(q);
}

int main() {
    FILE *input;
    FILE *output;
    input = fopen("tema1.in", "r");
    output = fopen("tema1.out", "w");
    Tren *tren = (Tren*)malloc(sizeof(Tren)); // aloc memorie pentru tren
    tren->locomotiva = (Vagon*)malloc(sizeof(Vagon)); // aloc memorie pentru locomotiva
    tren->locomotiva->next = NULL;
    tren->locomotiva->prev = NULL;
    Vagon *primul = (Vagon*)malloc(sizeof(Vagon)); // aloc memorie pentru primul vagon
    // strcpy(primul->nume, "#"); // setez numele primului vagon
    primul->next = tren->locomotiva; // fac legaturile
    tren->locomotiva->next = primul;
    tren->locomotiva->prev = primul;
    primul->prev = tren->locomotiva;
    tren->mecanic = primul;
    WRITE(tren, "#");
    char linie[20], *comanda, *argument;
    int n;
    Queue* q;
    q = createQueue();
    fscanf(input, "%d\n", &n);
    int s = 0; // variabila care sa retina daca s-a facut SWITCH sau nu
    while(fgets(linie, sizeof(linie), input)) {
        if (linie[strlen(linie) - 1] == '\n') // elimin "\n" care este adaugat dupa ce apelez functia fgets
            linie[strlen(linie) - 1] = '\0';
        if (strcmp(linie, "EXECUTE") != 0 && strcmp(linie, "SWITCH") != 0 && strcmp(linie, "SHOW") != 0 && strcmp(linie, "SHOW_CURRENT") != 0 && s == 0)
            enqueue(q, linie); // adaug comenzile la finalul cozii cand SWITCH-ul nu este activ
        else if (strcmp(linie, "EXECUTE") != 0 && strcmp(linie, "SWITCH") != 0 && strcmp(linie, "SHOW") != 0 && strcmp(linie, "SHOW_CURRENT") != 0 && s == 1)
            enqueue2(q, linie); // // adaug comenzile la finalul cozii cand SWITCH-ul este activ cu o alta functie (enqueue2)
        else if (strcmp(linie, "EXECUTE") == 0) {
            if (isQueueEmpty(q) == 0) {
                if (s == 0) { // daca nu s-a efectuat SWITCH-ul, iau prima comanda din coada
                    comanda = strtok(q->front->comanda, " ");
                    argument = strtok(NULL, " ");
                }
                else if (s == 1) { // daca s-a efectuat SWITCH-ul, iau ultima comanda din coada
                    comanda = strtok(q->rear->comanda, " ");
                    argument = strtok(NULL, " ");
                }
                if (strcmp(comanda, "MOVE_LEFT") == 0 && s == 0) { // cazurile cu s = 0 sunt pentru momentele cand nu am intalnit comanda SWITCH
                    MOVE_LEFT(tren);
                    dequeue(q); // elimin primul element din coada
                }
                else if (strcmp(comanda, "MOVE_RIGHT") == 0 && s == 0) {
                    MOVE_RIGHT(tren);
                    dequeue(q);
                }
                else if (strcmp(comanda, "WRITE") == 0 && s == 0) {
                    WRITE(tren, argument);
                    dequeue(q);
                }
                else if (strcmp(comanda, "CLEAR_CELL") == 0 && s == 0) {
                    CLEAR_CELL(tren);
                    dequeue(q);
                }
                else if (strcmp(comanda, "CLEAR_ALL") == 0 && s == 0) {
                    CLEAR_ALL(tren);
                    dequeue(q);
                }
                else if (strcmp(comanda, "INSERT_LEFT") == 0 && s == 0) {
                    INSERT_LEFT(tren, argument, output);
                    dequeue(q);
                }
                else if (strcmp(comanda, "INSERT_RIGHT") == 0 && s == 0) {
                    INSERT_RIGHT(tren, argument);
                    dequeue(q);
                }
                else if (strcmp(comanda, "SEARCH") == 0 && s == 0) {
                    SEARCH(tren, argument, output);
                    dequeue(q);
                }
                else if (strcmp(comanda, "SEARCH_RIGHT") == 0 && s == 0) {
                    SEARCH_RIGHT(tren, argument, output);
                    dequeue(q);
                }
                else if (strcmp(comanda, "SEARCH_LEFT") == 0 && s == 0) {
                    SEARCH_LEFT(tren, argument, output);
                    dequeue(q);
                }
                else if (strcmp(comanda, "MOVE_LEFT") == 0 && s == 1) { // cazurile pentru cand am intalnit comanda SWITCH
                    MOVE_LEFT(tren);
                    dequeue2(q); // elimin ultimul element din coada cu o functie diferita (dequeue2)
                }
                else if (strcmp(comanda, "MOVE_RIGHT") == 0 && s == 1) {
                    MOVE_RIGHT(tren);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "WRITE") == 0 && s == 1) {
                    WRITE(tren, argument);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "CLEAR_CELL") == 0 && s == 1) {
                    CLEAR_CELL(tren);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "CLEAR_ALL") == 0 && s == 1) {
                    CLEAR_ALL(tren);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "INSERT_LEFT") == 0 && s == 1) {
                    INSERT_LEFT(tren, argument, output);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "INSERT_RIGHT") == 0 && s == 1) {
                    INSERT_RIGHT(tren, argument);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "SEARCH") == 0 && s == 1) {
                    SEARCH(tren, argument, output);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "SEARCH_RIGHT") == 0 && s == 1) {
                    SEARCH_RIGHT(tren, argument, output);
                    dequeue2(q);
                }
                else if (strcmp(comanda, "SEARCH_LEFT") == 0 && s == 1) {
                    SEARCH_LEFT(tren, argument, output);
                    dequeue2(q);
                }
            }
        }
        else if (strcmp(linie, "SWITCH") == 0) {
            if (s == 0) // daca SWITCH-ul nu este pornit si se intalneste comanda SWITCH, s se transforma in 1
                s = 1;
            else if (s == 1) // daca SWITCH-ul este pornit si se intalneste comanda SWITCH, s se transforma in 0
                s = 0;
            }
        else if (strcmp(linie, "SHOW") == 0)
            SHOW(tren, output);
        else if (strcmp(linie, "SHOW_CURRENT") == 0)
            SHOW_CURRENT(tren, output);
    }

    Vagon *temp, *iter = tren->locomotiva->next; // eliberez memoria pentru tren si vagoane
    while (iter != tren->locomotiva) {
        temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(tren->locomotiva);
    free(tren);
    destroyQueue(q); // eliberarea memoriei pentru coada
    fclose(input); // inchid fisierele deschise mai sus
    fclose(output);
    return 0;
}
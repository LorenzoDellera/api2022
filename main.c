#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define leng 100

//STRUTTURA
typedef struct Lista *lista_punt;
struct Lista {
    char word[leng];
    int t0f1;
    lista_punt next;
};

//INSERIMENTO IN LISTA
lista_punt inserisci(lista_punt lista, char word[leng]) {
    lista_punt  new = NULL;
    new = (lista_punt) malloc(sizeof (struct Lista));
    strcpy(new->word,word);
    new->next = lista;
    return new;
}

//STAMPA LISTA
void stampa(lista_punt lista) {
    if (lista->next != NULL) {
        stampa(lista->next);
    }
    printf("%s",lista->word);
}

//PROGRAMMA
int main() {
    char string_prova[leng];
    int string_lenght, num_iniziale= 0;
    int disable = 0;                  //0=iniziale, 1=comandi base, 2=inserimemti;
    int num_inserimenti = 0;
    int num_stampe = 0;
    lista_punt lista = NULL;

    scanf("%d",&string_lenght);
    fgets(string_prova,leng,stdin);                           //todo: parsing di fgets invece di scanf e fgets.
    while (fgets(string_prova,leng,stdin) != NULL) {
        if(string_prova[0] == '+') {
            disable = 1;
            if (string_prova[1] == 'i' && string_prova[11] == 'i') {
                fgets(string_prova,leng,stdin);
                disable = 2;
            }
            if (string_prova[1] == 'i' && string_prova[11] == 'f') {
                disable = 1;
            }
            if (string_prova[1] == 's') {
                stampa(lista);
                num_stampe++;
            }
        }
        if(disable== 0 || disable==2) {
            lista = inserisci(lista,string_prova);
        }
        if(disable==0) {
            num_iniziale++;
        }
        if(disable==2) {
            num_inserimenti++;
        }
        /*if(string_prova[0] == '+' && string_prova[1] == 'i') {
            fgets(string_prova,leng,stdin);
            while (string_prova[0] != '+' && string_prova[1] != 'i') {
                puts(string_prova);
                fgets(string_prova,leng,stdin);
            }
        }*/
    }

    printf("parole iniziali = %d",num_iniziale);
    printf("\ndi lunghezza = %d",string_lenght);
    printf("\nparole inserite = %d",num_inserimenti);
    printf("\nstampe eseguite = %d",num_stampe);
}


//nuova_partita means parola chiave + numero tentativi = massimo righe da leggere se non comandi
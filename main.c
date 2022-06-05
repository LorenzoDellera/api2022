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
    int string_lenght, count=0;
    int disable = 0;
    lista_punt lista = NULL;

    scanf("%d",&string_lenght);
    fgets(string_prova,leng,stdin);
    while (fgets(string_prova,leng,stdin) != NULL) {
        if(string_prova[0] == '+') {
            disable = 1;
        }
        if(disable==0) {
            lista = inserisci(lista,string_prova);
        }
        if(disable==0) {
            count++;
        }
        /*if(string_prova[0] == '+' && string_prova[1] == 'i') {
            fgets(string_prova,leng,stdin);
            while (string_prova[0] != '+' && string_prova[1] != 'i') {
                puts(string_prova);
                fgets(string_prova,leng,stdin);
            }
        }*/
    }
    stampa(lista);
    printf("parole iniziali = %d",count);
    printf("\ndi lunghezza = %d",string_lenght);
}


//nuova_partita means parola chiave + numero tentativi = massimo righe da leggere se non comandi
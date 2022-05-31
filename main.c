#include <stdio.h>
#define leng 100

int main() {
    char string_prova[leng];
    int string_lenght, count=0;
    int disable = 0;

    scanf("%d",&string_lenght);
    fgets(string_prova,leng,stdin);
    while (fgets(string_prova,leng,stdin) != NULL) {
        if(string_prova[0] == '+') {
            disable = 1;
        }
        if(disable==0) {
            puts(string_prova);
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
    printf("parole iniziali = %d",count);
    printf("\ndi lunghezza = %d",string_lenght);
}


//nuova_partita means parola chiave + numero tentativi = massimo righe da leggere se non comandi
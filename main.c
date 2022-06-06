#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define line_length 50

//STRUCT
typedef struct List *list_punt;
struct List {
    char word[line_length];
    int booleanF0T1;     //TRUE = 1, FALSE = O;
    list_punt next;
};

//LIST INSERTION
list_punt list_insertion(list_punt actual_list, char new_word[line_length]) {
    list_punt  new_list = NULL;

    new_list = (list_punt) malloc(sizeof (struct List));
    strcpy(new_list->word,new_word);
    new_list->next = actual_list;
    return new_list;
}

//LIST PRINTER
void list_print(list_punt list) {
    if (list->next != NULL) {
        list_print(list->next);
    }
    printf("%s",list->word);
}

//CUSTOM PARSING
int parsing(const char word[line_length]) {
    int number = 0;

    for (int i = 0; word[i] !=  '\n'; i++) {
        number = number*10 + word[i] - '0';
    }
    return number;
}

//NUMBER READER
int number_reader() {
    int number;
    char garbage_string[line_length];

    number = parsing(fgets(garbage_string,line_length,stdin));

    return number;
}

//MODE CHECKER
int mode_check(char new_word[line_length], int actual_mode) {
    //MODE: 0 = initial_mode (initial dictionary), 1 = commands_phase ('+_something'), 2 = insertions, 3 = print, 4 = new_game;
    if (actual_mode == 3 || actual_mode == 4) {
        actual_mode = 1;
    }

    if(new_word[0] == '+') {
        //+inserisci_inizio/fine
        if (new_word[1] == 'i') {
            if (actual_mode == 1) {
                fgets(new_word, line_length, stdin);
                actual_mode = 2;
            }
            else if (actual_mode == 2) {
                actual_mode = 1;
            }
        }
        //stampa_filtrate
        if (new_word[1] == 's') {
            actual_mode = 3;
        }
        //+nuova_partita
        if (new_word[1] == 'n') {
            actual_mode = 4;
        }
    }

    return actual_mode;
}

//MAIN
int main() {
    char string_placeholder[line_length];
    char key_word[line_length];
    int string_length;
    int max_attempts;
    int mode = 0;     //MODE: 0 = initial_mode (initial dictionary), 1 = commands_phase ('+_something'), 2 = insertions, 3 = new_game;
    list_punt list = NULL;

    int initial_number = 0;     //testing;
    int insertions_number = 0;     //testing;
    int print_number = 0;     //testing;

    //string_length reader
    string_length = number_reader();

    //INPUT reader
    while (fgets(string_placeholder,line_length,stdin) != NULL) {
        //checking mode
        mode = mode_check(string_placeholder,mode);
        //insertions (initial & not)
        if (mode == 0 || mode == 2) {
            list = list_insertion(list,string_placeholder);
        }
        if (mode == 0) {     //testing;
            initial_number++;
        }
        if (mode == 2) {     //testing;
            insertions_number++;
        }
        //+stampa_filtrate
        if (mode == 3) {
            list_print(list);
            print_number++;
            mode = 1;
        }
        //+nuova_partita
        if (mode == 4) {
            fgets(key_word,line_length,stdin);
            max_attempts = number_reader();
            mode = 1;
            //TODO:
        }
    }

    printf("parole iniziali = %d",initial_number);     //testing;
    printf("\ndi lunghezza = %d",string_length);     //testing;
    printf("\nparole inserite = %d",insertions_number);     //testing;
    printf("\nstampe = %d",print_number);     //testing;
    printf("\nparola chiave: %s",key_word);     //testing;
    printf("numeri di tentativi: %d",max_attempts);     //testing;
}


//lettura input non va sempre bene, insert.txt e test1.txt funzionano, slide.txt NO --> non inserisce le prime 3 parole (+inserisici_inizio) e stampa classifiche in più random
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

    new_list = (list_punt) malloc(sizeof(struct List));
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

//LIST COUNTER
int list_count(list_punt list) {     //TODO: to be fixed with booleanF0T1
    int number = 0;
    if (list != NULL) {
        number++;
        if (list->next != NULL) {
            number = number + list_count(list->next);
        }
    }

    return number;
}

//CHECK IF WORD IS PRESENT IN LIST
int if_present(list_punt list, const char word[line_length]) {
    int presence = 0;     //0 = not_exists, 1 = is_present

    if (list->next != NULL) {
        presence = if_present(list->next,word);
    }
    if (strcmp(list->word,word) == 0) {
        presence = 1;
    }

    return presence;
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

//KEY WORD SETTER
void key_word_setter(char key[line_length], const char string[line_length], int length) {
    for (int i=0;i<length;i++) {
        key[i] = string[i];
    }
}

//TRY WORD SETTER
void try_word_setter(char try[line_length], const char string[line_length], int length) {
    for (int i=0;i<length;i++) {
        try[i] = string[i];
    }
}

//CHECK + OK
int check_ok(const char string[line_length], int length) {
    int count = 0;
    for (int i=0;i<length;i++) {
        if (string[i] == '+') {
            count++;
        }
    }
    if (count==length) {
        return 1;         //1 = yes
    }
    else {
        return 0;         //0 = no
    }
}

//WORD COMPARATOR
void compare_word(const char
key[line_length], const char try[line_length], int length, list_punt list) {
    int key_occurrence[line_length];
    char try_output[line_length];

    //initialize key_occurrence
    for (int zero=0;zero<length;zero++) {
        key_occurrence[zero] = 0;
    }

    //initialize try_output
    for (int bar=0;bar<length;bar++) {
        try_output[bar] = '/';
    }
    try_output[length] = '\n';

    //comparing (searching for  '+')
    for (int i=0;i<length;i++) {
        if (try[i] == key[i]) {
            try_output[i] = '+';
            key_occurrence[i] = 1;
        }
    }

    //comparing (searching for  '|')
    for (int i=0;i<length;i++) {     //looking try array
        for (int j=0;j<length;j++) {     //looking key array
            if (try_output[i] == '/') {
                if ((try[i] == key[j]) && (key_occurrence[j] == 0)) {
                    key_occurrence[j] = 1;
                    try_output[i] = '|';
                }
            }
        }
    }

    //printing
    if (check_ok(try_output,length) == 1) {
        printf("ok\n");
    }
    else {
        for (int pr=0;pr<=length;pr++) {
            printf("%c",try_output[pr]);
        }          //TODO: update list with booleanF0T1
        printf("%d\n", list_count(list));
    }
}

//MAIN
int main() {
    char string_placeholder[line_length];
    int string_length;
    int max_attempts;
    int mode = 0;     //MODE: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries;
    list_punt list = NULL;


    char key_word[line_length];
    char try_word[line_length];

    //testing
    int initial_number = 0;
    int insertions_number = 0;
    int print_number = 0;

    //string_length reader
    string_length = number_reader();

    //INPUT reader
    while (fgets(string_placeholder,line_length,stdin) != NULL) {
        //command
        if (string_placeholder[0] == '+') {
            //+insert_start
            if (string_placeholder[1] == 'i' && string_placeholder[11] == 'i') {
                mode = 1;
            }
            //+insert_end
            if (string_placeholder[1] == 'i' && string_placeholder[11] == 'f') {
                mode = 3;
            }
            //print_filtrate
            if (string_placeholder[1] == 's') {
                list_print(list);
                print_number++;
                mode = 2;
            }
            //+new_game
            if (string_placeholder[1] == 'n') {
                if (fgets(string_placeholder, line_length, stdin) != NULL) {
                    key_word_setter(key_word, string_placeholder, string_length + 1);
                    max_attempts = number_reader();
                    mode = 2;
                }
            }
        }
        //word
        else {     //mode: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries;
            //initial dictionary
            if (mode == 0) {
                list = list_insertion(list,string_placeholder);
                initial_number++;
            }
            //insertions
            if (mode == 1) {
                list = list_insertion(list,string_placeholder);
                insertions_number++;
            }
            //tries
            if (mode == 2) {
                try_word_setter(try_word,string_placeholder,string_length+1);
                if (if_present(list,try_word) == 1) {
                    printf("------------\n");          //testing
                    compare_word(key_word,try_word,string_length,list);
                    printf("------------\n");          //testing
                }
                else if (if_present(list,try_word) == 0) {
                    printf("not_exists\n");
                }
            }
        }
    }

    //testing
    printf("initial words = %d",initial_number);
    printf("\nwords length = %d",string_length);
    printf("\nnew words = %d",insertions_number);
    printf("\ntotal words = %d", list_count(list));
    printf("\nprints number = %d",print_number);
    printf("\nkey word: %s",key_word);
    printf("number of tries: %d",max_attempts);
}

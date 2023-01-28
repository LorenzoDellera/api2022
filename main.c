#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define line_length 30

//STRUCT
typedef struct List *list_punt;
struct List {
    char word[line_length];
    char output[line_length];
    int printable;     //FALSE = O, TRUE = 1;
    list_punt next;
};

//RESET LIST
void reset_list(list_punt list) {
    if (list->next != NULL) {
        reset_list(list->next);
    }
    list->printable = 1;
    list->output[0] = 'n';
}

//ALPHABETIC ORDER         //TODO: FIX THIS
void order_list(list_punt list) {
    char temp_word[line_length];
    char temp_output[line_length];
    int temp_num;

    if (list->next != NULL) {
        if (strcmp(list->word,list->next->word) < 0) {
            if (list->printable != list->next->printable) {
                temp_num = list->printable;
                list->printable = list->next->printable;
                list->next->printable = temp_num;
            }
            if (list->output != list->next->output) {
                strcpy(temp_output,list->output);
                strcpy(list->output,list->next->output);
                strcpy(list->next->output,temp_output);

            }
            strcpy(temp_word,list->word);
            strcpy(list->word,list->next->word);
            strcpy(list->next->word,temp_word);
        }
        order_list(list->next);
    }
}
//node is the new_node in the head of the list, head_list is the second element (first of the ordered list)
void list_sorting(list_punt node, list_punt first_ordered_list, list_punt prev_list) {
    if(node != NULL && first_ordered_list != NULL) {
        if(strcmp(node->word,first_ordered_list->word) < 0) {
            list_sorting(node,first_ordered_list->next,first_ordered_list);
        }
    }
    else {
        prev_list->next = node;
        node->next = first_ordered_list;
    }
}

//LIST INSERTION
list_punt list_insertion(list_punt actual_list, char new_word[line_length]) {
    list_punt  new_list = NULL;

    new_list = (list_punt) malloc(sizeof(struct List));
    strcpy(new_list->word,new_word);
    new_list->printable = 1;
    strcpy(new_list->output,"n");
    new_list->next = actual_list;
    order_list(new_list);
    return new_list;
}

//LIST INSERTION
list_punt ordered_list_insertion(list_punt actual_list, char new_word[line_length]) {
    list_punt  new_node = NULL;

    //first element
    if (actual_list == NULL) {
        actual_list = (list_punt) malloc(sizeof(struct List));
        strcpy(actual_list->word,new_word);
        actual_list->printable = 1;
        strcpy(actual_list->output,"n");
        actual_list->next = NULL;
    }
    else {
        if (strcmp(new_word,actual_list->word) < 0) {
            if(strcmp(new_word,actual_list->next->word) < 0) {
                ordered_list_insertion(actual_list->next,new_word);
            }
            else {
                new_node = (list_punt) malloc(sizeof(struct List));
                strcpy(new_node->word,new_word);
                new_node->printable = 1;
                strcpy(new_node->output,"n");
                new_node->next = actual_list->next;
                actual_list->next = new_node;
            }
        }
            //only if the element is the first in alphabetic order
        else {
            new_node = (list_punt) malloc(sizeof(struct List));
            strcpy(new_node->word,new_word);
            new_node->printable = 1;
            strcpy(new_node->output,"n");
            new_node->next = actual_list;
        }
    }
    return actual_list;
}

//LIST INSERTION SORTED
void insert(list_punt *pl,char string[line_length]) {
    list_punt s = NULL;
    list_punt r = NULL;

    // la lista non contiene elementi
    if(*pl==NULL) {
        *pl=malloc(sizeof(struct List));
        strcpy((*pl)->word,string);
        (*pl)->printable = 1;
        strcpy((*pl)->output,"n");
        (*pl)->next = NULL;
        return;
    }

    else {
        // l'elemento va inserito in prima posizione
        if(strcmp(string,(*pl)->word) > 0) {
            s = *pl;
            *pl = malloc(sizeof(struct List));
            strcpy((*pl)->word,string);
            (*pl)->printable = 1;
            strcpy((*pl)->output,"n");
            (*pl)->next = s;
            return;
        }

        // la lista ha piu' di un elemento
        s=*pl;
        while(s->next != NULL) {
            if(strcmp(string,s->next->word) > 0) {
                r = s->next;
                s->next = malloc(sizeof(struct List));
                strcpy(s->next->word,string);
                s->next->printable = 1;
                strcpy(s->next->output,"n");
                s->next->next = r;
                return;
            }
            s = s->next;
        }

        // se arrivo qui, l'elemento va inserito in fondo
        s->next = malloc(sizeof(struct List));
        strcpy(s->next->word,string);
        s->next->printable = 1;
        strcpy(s->next->output,"n");
        s->next->next = NULL;

        return;
    }
}

//LIST PRINTER
void list_print(list_punt list, int lenght) {
    if (list == NULL) {
        //printf("empty list\n");   //testing
    }
    else {
        if (list->next != NULL) {
            list_print(list->next,lenght);
        }
        if (list->printable == 1) {
            for (int i=0;i < lenght; i++) {
                putchar(list->word[i]);
            }
            putchar('\n');
            //printf("%d  %s",list->printable,list->word);       //testing
        }
    }
}

//LIST COUNTER
int list_count(list_punt list) {
    int number = 0;
    if (list != NULL) {
        if (list->printable == 1) {
            number++;
        }
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

//SET THE OUTPUT OF THE WORD IN LIST
void set_output(list_punt list, const char word[line_length], const char output[line_length]) {
    if (list->next != NULL) {
        set_output(list->next, word,output);
    }
    if (strcmp(list->word, word) == 0) {
        strcpy(list->output,output);
    }
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

//CHECK SYMBOL + IN LIST
void check_plus_list(list_punt list, int index, const char try[line_length]) {
    if (list->next != NULL) {
        check_plus_list(list->next,index,try);
    }
    if (list->printable == 1) {
        if (list->word[index] != try[index]) {
            list->printable = 0;
        }
    }
}

//CHECK SYMBOL / IN LIST
void check_sidebar_list(list_punt list, int length, const char try[line_length], int index) {
    if (list->next != NULL) {
        check_sidebar_list(list->next,length,try,index);
    }

    int count_try = 0;
    int count_list_word = 0;
    if (list->printable == 1) {
        if (try[index] == list->word[index]) {
            list->printable = 0;
        }
        else {
            for (int i=0;i<length;i++) {   //comparing all the word to find if there is a wrong litter
                if (list->word[i] == try[index]) {
                    count_list_word++;
                }
                if (i<index) {
                    if (try[i] == try[index]) {
                        count_try++;
                    }
                }
            }
            if (count_list_word != 0) {
                if (count_list_word != count_try) {
                    list->printable = 0;
                }
            }
        }
    }
}

//CHECK SYMBOL | IN LIST
void check_straight_bar_list(list_punt list, int length, const char try[line_length], int index) {
    if (list->next != NULL) {
        check_straight_bar_list(list->next,length,try,index);
    }

    int count_try = 0;
    int count_list_word = 0;
    if (list->printable == 1) {
        if (try[index] == list->word[index]) {
            list->printable = 0;
        }
        else {
            for (int i=0;i<length;i++) {   //comparing all the word to find if there is a wrong litter
                if (list->word[i] == try[index]) {
                    count_list_word++;
                }
                if (i<=index) {
                    if (try[i] == try[index]) {
                        count_try++;
                    }
                }
            }
            if (count_list_word < count_try) {
                list->printable = 0;
            }
        }
    }
}

//LIST UPDATE WITH BOUNDS
void list_bounds_check(list_punt list, int length, const char try[line_length], const char try_output[line_length]) {
    for (int i=0;i<length;i++) {     //looking each symbol in try_output
        //looking litter in + position
        if (try_output[i] == '+') {
            check_plus_list(list,i,try);
        }
        //looking for litter in / position
        if (try_output[i] == '/') {
            check_sidebar_list(list,length,try,i);
        }
        //looking for litter in wrong position |
        if (try_output[i] == '|') {
            check_straight_bar_list(list,length,try,i);
        }
    }
}

//LIST GENERAL UPDATE WITH BOUNDS
void general_list_bounds_check(list_punt list, int length) {
    if (list->next != NULL) {
        general_list_bounds_check(list->next,length);
    }
    if (strcmp(list->output,"n") != 0) {
        list_bounds_check(list,length,list->word,list->output);
    }
}

//WORD COMPARATOR
void compare_word(const char key[line_length], const char try[line_length], int length, list_punt list, int number_attempts) {
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

    set_output(list,try,try_output);
    list_bounds_check(list,length,try,try_output);
    //printf("comparison has been done\n");   //testing

    //printing
    if (check_ok(try_output,length) == 1) {
        puts("ok");
    }
    else {
        for (int pr=0;pr<=length;pr++) {
            putchar(try_output[pr]);
        }
        if (list_count(list) == 0) {
            printf("1\n");
        }
        else {
            printf("%d\n", list_count(list));
        }
        if (number_attempts ==0) {
            puts("ko");
        }
    }
}

//MAIN
int main() {
    char string_placeholder[line_length];
    int string_length;
    int max_attempts;
    int actual_attempts;
    int mode = 0;     //MODE: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries;
    list_punt list = NULL;

    char key_word[line_length];
    char try_word[line_length];

    //testing
    //int initial_number = 0;
    //int insertions_number = 0;
    //int print_number = 0;

    //string_length reader
    string_length = number_reader();

    //INPUT reader
    while (fgets(string_placeholder, line_length, stdin) != NULL) {
        //command
        if (string_placeholder[0] == '+') {
            //insert
            if (string_placeholder[1] == 'i') {
                //+insert_start
                if (string_placeholder[11] == 'i') {
                    mode = 1;
                    //printf("insertion start\n");   //testing
                }
                    //+insert_end
                else if (string_placeholder[11] == 'f') {
                    mode = 2;
                    //printf("insertion end\n");   //testing
                }
            }
                //print_filtrate
            else if (string_placeholder[1] == 's') {
                //printf("------------\n");          //testing
                //order_list(list);     //TODO: FIX ORDER ALGORITHM TO ORDER THE LIST ONCE IF THE WORDS ARE VALID TO BE FILTRATE
                list_print(list, string_length);
                //printf("------------\n");          //testing
                //print_number++;   //testing
                mode = 2;
            }
                //+new_game
            else if (string_placeholder[1] == 'n') {
                if (fgets(string_placeholder, line_length, stdin) != NULL) {
                    key_word_setter(key_word, string_placeholder, string_length + 1);
                    max_attempts = number_reader();
                    actual_attempts = max_attempts;
                    reset_list(list);
                    mode = 2;
                }
            }
        }
            //word
        else {     //mode: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries;
            //initial dictionary
            if (mode == 0) {
                insert(&list,string_placeholder);
                ///list = list_insertion(list, string_placeholder);
                //initial_number++;
            }
                //insertions
            else if (mode == 1) {
                insert(&list,string_placeholder);
                ///list = list_insertion(list, string_placeholder);
                //list = ordered_list_insertion(list,string_placeholder);
                if (actual_attempts != max_attempts) {
                    general_list_bounds_check(list, string_length);
                }
                //insertions_number++;   //testing
            }
                //tries
            else if (mode == 2) {
                try_word_setter(try_word, string_placeholder, string_length + 1);
                if (if_present(list, try_word) == 1) {
                    //printf("------------\n");          //testing
                    actual_attempts--;
                    compare_word(key_word, try_word, string_length, list, actual_attempts);
                    //printf("remaining attempts: %d\n",max_attempts);   //testing
                    //printf("------------\n");          //testing
                } else if (if_present(list, try_word) == 0) {
                    printf("not_exists\n");
                    //printf("remaining attempts: %d\n",max_attempts);   //testing
                }
            }
        }
    }

    //testing
    //printf("initial words = %d\n",initial_number);
    //printf("words length = %d\n",string_length);
    //printf("new words = %d\n",insertions_number);
    //int total = initial_number + insertions_number;
    //printf("total words = %d\n", total);
    //printf("prints number = %d\n",print_number);
    //printf("key word: %s\n",key_word);
    //printf("number of tries: %d\n",max_attempts);
}
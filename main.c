#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define line_length 20

/// TREE STRUCT
typedef struct Tree *tree_punt;
struct Tree {
    char word[line_length];
    // 0=false
    // 1=true
    int printable;
    tree_punt sx;
    tree_punt dx;
};

/// LIST STRUCT
typedef struct List *list_punt;
struct List {
    char word[line_length];
    char output[line_length];
    // 0=false
    // 1=true
    int valid_in_game;
    list_punt next;
};

/// CUSTOM STRING COPY
void custom_string_copy(char destination[line_length], const char string[line_length], int length) {
    for (int i = 0;i <= length;i++) {
        destination[i] = string[i];
    }
}

/// NEW NODE CREATION
tree_punt new_node_tree(char string[line_length], int printable, int length) {
    tree_punt new
            = (tree_punt)malloc(sizeof(struct Tree));
    new->printable = printable;
    custom_string_copy(new->word,string,length);
    new->sx = new->dx = NULL;
    return new;
}

/// NEW NODE CREATION
list_punt new_node_list(char word[line_length], char output[line_length], int length) {
    list_punt new
            = (list_punt)malloc(sizeof (struct List));
    new->valid_in_game = 1;
    custom_string_copy(new->word,word,length);
    custom_string_copy(new->output,output,length);
    new->next =NULL;
    return new;
}

/// TREE INSERTION
tree_punt tree_insert(tree_punt T, char string[line_length], int printable, int length) {
    // empty tree
    if (T == NULL)
        return new_node_tree(string,printable,length);
    // searching along the tree
    if (strcmp(string,T->word) < 0) {
        T->sx = tree_insert(T->sx,string,printable,length);
    }
    else {
        T->dx = tree_insert(T->dx,string,printable,length);
    }
    return T;
}

/// LIST INSERTION
list_punt list_insert(list_punt L, char word[line_length], char output[line_length], int length) {
    list_punt new = new_node_list(word,output,length);
    new->next = L;
    return new;
}

/// TREE ELEMENTS COUNTER
int tree_counter(tree_punt T) {
    int number = 0;
    if (T == NULL) {
        return number;
    }
    else {
        number = number + tree_counter(T->sx);
        if (T->printable == 1) {
            number++;
        }
        number = number + tree_counter(T->dx);
    }
    return number;
}

/// PRINT TREE
void tree_print(tree_punt T, int length) {
    if (T != NULL) {
        tree_print(T->sx,length);
        if (T->printable == 1) {
            for (int i=0;i <= length;i++) {
                putchar(T->word[i]);
            }
        }
        tree_print(T->dx,length);
    }
}

/// PRINT LIST
void list_print(list_punt L, int length) {
    if (L != NULL) {
        if (L->valid_in_game == 1) {
            for (int i=0;i <= length;i++) {
                putchar(L->word[i]);
            }
        }
        list_print(L->next,length);
    }
}

/// RESET TREE
void tree_reset(tree_punt T) {
    if (T != NULL) {
        tree_reset(T->sx);
        T->printable = 1;
        tree_reset(T->dx);
    }
}

/// RESET LIST
void list_reset(list_punt L) {
    if (L != NULL) {
        if (L->valid_in_game == 1) {
            L->valid_in_game = 0;
        }
        list_reset(L->next);
    }
}

/// FREE TREE
void tree_free(tree_punt T) {
    if (T != NULL) {
        tree_free(T->sx);
        tree_free(T->dx);
        free(T);
    }
}

/// FREE LIST
void list_free(list_punt L) {
    if (L != NULL) {
        list_free(L->next);
        free(L);
    }
}

/// FIND WORD INTO THE TREE
// 1 = yes
// 0 = no
int tree_find_word(tree_punt T, char string[line_length]) {
    int number = 0;
    // empty tree -> 0
    if (T == NULL) {
        return number;
    }
    else {
        if (T->sx != NULL) {
            number = number + tree_find_word(T->sx,string);
        }
        if (strcmp(string,T->word) == 0) {
            number = 1;
        }
        if (T->dx != NULL) {
            number = number + tree_find_word(T->dx,string);
        }
    }
    // after researching in all the tree
    return number;
}

/// FIND WORD INTO THE LIST
// 1 = yes
// 0 = no
int list_find_word(list_punt L, char string[line_length]) {
    // empty list -> 0
    if (L == NULL) {
        return 0;
    }
    else {
        if (strcmp(string,L->word) == 0) {
            return 1;
        }
        if (L->next != NULL) {
            list_find_word(L->next,string);
        }
    }
    // after researching in all the list
    return 0;
}

/// LIST OUTPUT SETTER
void list_set_output(list_punt L, char string[line_length], char output[line_length], int length) {
    if (L == NULL) {
        return;
    }
    else {
        if (strcmp(string,L->word) == 0) {
            custom_string_copy(L->output,output,length);
            L->valid_in_game = 1;
        }
        if (L->next != NULL) {
            list_find_word(L->next,string);
        }
    }
}

/// CUSTOM PARSING
int parsing(const char word[line_length]) {
    int number = 0;

    for (int i = 0; word[i] !=  '\n'; i++) {
        number = number*10 + word[i] - '0';
    }
    return number;
}

/// CHECK ACCURACY TRY
// 1 = right
// 0 = wrong
int check_try(const char output[line_length], int length) {
    int ok = 0;
    for (int i = 0;i < length;i++) {
        if (output[i] == '+') {
            ok++;
        }
    }
    if (ok == length) {
        return 1;
    }
    else return 0;
}

/// OUTPUT GENERATOR
void output_generator(const char key[line_length], const char try[line_length], char try_output[line_length], int length) {
    int key_occurrences[line_length];

    // initialize key_occurrence
    for (int zero = 0;zero < length;zero++) {
        key_occurrences[zero] = 0;
    }

    // initialize output_occurrences
    for (int bar=0;bar<length;bar++) {
        try_output[bar] = '/';
    }
    try_output[length] = '\n';

    // comparing (searching for '+')
    for (int i=0;i<length;i++) {
        if (try[i] == key[i]) {
            try_output[i] = '+';
            key_occurrences[i] = 1;
        }
    }

    // comparing (searching for '|')
    // looking try array
    for (int i=0;i<length;i++) {
        // looking key array
        for (int j=0;j<length;j++) {
            if (try_output[i] == '/') {
                if ((try[i] == key[j]) && (key_occurrences[j] == 0)) {
                    key_occurrences[j] = 1;
                    try_output[i] = '|';
                }
            }
        }
    }
}

/// WORD-TREE COMPARATOR
void tree_comparator(tree_punt T, char word[line_length], char output[line_length], int length) {
    if (T != NULL) {
        if (T->sx != NULL) {
            tree_comparator(T->sx,word,output,length);
        }
        if (T->printable == 1) {
            for (int i = 0;i < length;i++) {
                if (output[i] == '+') {
                    if (word[i] != T->word[i]) {
                        T->printable = 0;
                    }
                }
                else if (output[i] == '/') {
                    if (word[i] == T->word[i]) {
                        T->printable = 0;
                    }
                    else {
                        int count_word = 0;
                        int count_tree = 0;
                        for (int j = 0;j < length;j++) {
                            if (T->word[j] == word[i]) {
                                count_tree++;
                            }
                            if (word[j] == word[i]) {
                                if (output[j] == '|' || output[j] == '+') {
                                    count_word++;
                                }
                            }
                        }
                        if (count_tree != count_word) {
                            T->printable = 0;
                        }
                    }
                }
                else if (output[i] == '|') {
                    if (word[i] == T->word[i]) {
                        T->printable = 0;
                    }
                    else {
                        int count_word = 0;
                        int count_tree = 0;
                        for (int j = 0;j < length;j++) {
                            if (T->word[j] == word[i]) {
                                count_tree++;
                            }
                            if (word[j] == word[i]) {
                                if (output[j] == '|' || output[j] == '+') {
                                    count_word++;
                                }
                            }
                        }
                        if (count_word > count_tree) {
                            T->printable = 0;
                        }
                    }
                }
            }
        }
        if (T->dx != NULL) {
            tree_comparator(T->dx,word,output,length);
        }
    }
}

/// WORD-LIST COMPARATOR
// 1 = printable
// 0 = not_printable
int list_comparator(list_punt L, char string[line_length], int length) {
    if (L == NULL) {
        return 1;
    }
    else {
        if (L->valid_in_game == 1) {
            for (int i = 0;i < length;i++) {
                if (L->output[i] == '+') {
                    if (L->word[i] != string[i]) {
                        return 0;
                    }
                }
                else if (L->output[i] == '/') {
                    if (L->word[i] == string[i]) {
                        return 0;
                    }
                    else {
                        int count_string = 0;
                        int count_list = 0;
                        for (int j = 0;j < length;j++) {
                            if (string[j] == L->word[i]) {
                                count_string++;
                            }
                            if (L->word[j] == L->word[i]) {
                                if (j < i) {
                                    if (L->output[j] == '|') {
                                        count_list++;
                                    }
                                }
                                if (L->output[i] == '+') {
                                    count_list++;
                                }
                            }
                        }
                        if (count_list != count_string) {
                            return 0;
                        }
                    }
                }
                else if (L->output[i] == '|') {
                    if (L->word[i] == string[i]) {
                        return 0;
                    }
                    else {
                        int count_string = 0;
                        int count_list = 0;
                        for (int j = 0;j < length;j++) {
                            if (string[j] == L->word[i]) {
                                count_string++;
                            }
                            if (L->word[j] == L->word[i]) {
                                if (L->output[j] == '|' || L->output[i] == '+') {
                                    count_list++;
                                }
                            }
                        }
                        if (count_string < count_list) {
                            return 0;
                        }
                    }
                }
            }
            list_comparator(L->next,string,length);
        }
        else {
            list_comparator(L->next,string,length);
        }
    }
    return 1;
}

/// MAIN CODE
int main() {

    // local variables
    int string_length;
    int mode = 0;   // MODE: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries
    int correct;
    int max_attempts;
    int actual_attempts;
    char key_word[line_length];
    char try_word[line_length];
    char try_output[line_length];
    char string_placeholder[line_length];
    list_punt list = NULL;
    tree_punt tree = NULL;

    // string_length reader
    string_length = parsing(fgets(string_placeholder,line_length,stdin));
    //printf("%d",string_length);   // testing

    // INPUT reader
    while (fgets(string_placeholder, line_length, stdin) != NULL) {
        // command
        if (string_placeholder[0] == '+') {
            // insert
            if (string_placeholder[1] == 'i') {
                // +insert_start
                if (string_placeholder[11] == 'i') {
                    mode = 1;
                    //printf("insertion start\n");   // testing
                }
                    // +insert_end
                else if (string_placeholder[11] == 'f') {
                    mode = 2;
                    //printf("insertion end\n");   // testing
                }
            }
                // print_filtrate
            else if (string_placeholder[1] == 's') {
                //printf("------------\n");   // testing
                tree_print(tree,string_length);
                //printf("------------\n");   // testing
                //print_number++;   // testing
                mode = 2;
            }
                // +new_game
            else if (string_placeholder[1] == 'n') {
                if (fgets(key_word, line_length, stdin) != NULL) {
                    max_attempts = parsing(fgets(string_placeholder,line_length,stdin));
                    actual_attempts = max_attempts;
                    tree_reset(tree);
                    list_reset(list);
                    mode = 2;
                }
            }
        }
            // word
        else {   //mode: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries
            // initial dictionary
            if (mode == 0) {
                tree = tree_insert(tree,string_placeholder,1, string_length);
                //initial_number++;
            }
                // insertions
            else if (mode == 1) {
                tree = tree_insert(tree,string_placeholder, list_comparator(list,string_placeholder,string_length),string_length);
                //insertions_number++;   // testing
            }
                // tries
            else {
                custom_string_copy(try_word,string_placeholder,string_length);
                if (tree_find_word(tree,try_word) == 1) {
                    //printf("------------\n");   // testing
                    actual_attempts--;
                    output_generator(key_word,try_word,try_output,string_length);
                    correct = check_try(try_output,string_length);
                    if (correct == 1) {
                        puts("ok");
                    }
                    else {
                        for (int i = 0; i <= string_length;i++) {
                            putchar_unlocked(try_output[i]);
                        }
                        if (list_find_word(list,try_word) == 0) {
                            list = list_insert(list,try_word,try_output,string_length);
                        }
                        else {
                            list_set_output(list,try_word,try_output,string_length);
                        }
                        tree_comparator(tree,try_word,try_output,string_length);
                        printf("%d\n", tree_counter(tree));
                        if (actual_attempts == 0) {
                            puts("ko");
                        }
                        //printf("remaining attempts: %d\n",max_attempts);   // testing
                        //printf("------------\n");          //testing
                    }
                }
                else {
                    printf("not_exists\n");
                    //printf("remaining attempts: %d\n",actual_attempts);   // testing
                }
            }
        }
    }
    // free memory
    tree_free(tree);
    list_free(list);

    /// testing
    //printf("initial words = %d\n",initial_number);
    //printf("words length = %d\n",string_length);
    //printf("new words = %d\n",insertions_number);
    //int total = initial_number + insertions_number;
    //printf("total words = %d\n", total);
    //printf("prints number = %d\n",print_number);
    //printf("key word: %s\n",key_word);
    //printf("try word: %s\n",try_word);
    //printf("number of tries: %d\n",max_attempts);
}
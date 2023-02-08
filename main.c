#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define line_length 19

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

/// POINTERS LIST STRUCT
typedef struct Pointer *pointer_punt;
struct Pointer {
    tree_punt node;
    pointer_punt next;
};

/// NEW NODE CREATION
list_punt new_node_list(char word[line_length], char output[line_length]) {
    list_punt new
            = (list_punt)malloc(sizeof (struct List));
    new->valid_in_game = 1;
    strcpy(new->word,word);
    strcpy(new->output,output);
    new->next =NULL;
    return new;
}

/// CUSTOM STRING COMPARE
int custom_string_compare(char *s1, char *s2) {
    while(*s1 ==  *s2) {
        if (*s1 == 0 && *s2 == 0) {
            return 0;
        }
        s1++;
        s2++;
    }
    if (*s1 < *s2) {
        return -1;
    }
    else {
        return 1;
    }
}

/// NEW NODE CREATION
tree_punt new_node_tree(char string[line_length], int printable) {
    tree_punt new
            = (tree_punt)malloc(sizeof(struct Tree));
    new->printable = printable;
    strcpy(new->word,string);
    new->sx = new->dx = NULL;
    return new;
}

/// LIST INSERTION
list_punt list_insert(list_punt L, char word[line_length], char output[line_length]) {
    list_punt new = new_node_list(word,output);
    new->next = L;
    return new;
}

/// PRINT TREE
void tree_print(tree_punt T, int length) {
    if (T != NULL) {
        tree_print(T->sx,length);
        if (T->printable == 1) {
            fputs(T->word,stdout);
        }
        tree_print(T->dx,length);
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
    while (L != NULL) {
        L->valid_in_game = 0;
        L = L->next;
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

/// FREE POINTERS LIST
void pointer_free(pointer_punt P) {
    if (P != NULL) {
        pointer_free(P->next);
        free(P);
    }
}

/// CUSTOM PARSING (STRING TO INT)
int parsing(const char word[line_length]) {
    int number = 0;

    for (int i = 0;word[i] != '\n';i++) {
        number = number*10 + word[i] - '0';
    }
    return number;
}

/// FIND WORD INTO THE TREE
// 1 = yes
// 0 = no
int tree_find_word(tree_punt T, char string[line_length]) {
    int check;
    while (T != NULL) {
        check = custom_string_compare(string,T->word);
        if (check == 0) {
            return 1;
        }
        else if (check < 0) {
            T = T->sx;
        }
        else {
            T = T->dx;
        }
    }
    return 0;
}

/// FIND WORD INTO THE LIST
// 1 = yes
// 0 = no
int list_find_word(list_punt L, char string[line_length]) {
    while (L != NULL) {
        if (custom_string_compare(string,L->word) == 0) {
            return 1;
        }
        L = L->next;
    }
    return 0;
}

/// LIST OUTPUT SETTER
void list_set_output(list_punt L, char string[line_length], char output[line_length]) {
    while (L != NULL) {
        if (custom_string_compare(string,L->word) == 0) {
            strcpy(L->output,output);
            L->valid_in_game = 1;
            return;
        }
        L = L->next;
    }
}

/// CHECK ACCURACY TRY
// 1 = right
// 0 = wrong
int check_try(const char output[line_length], int length) {
    for (int i = 0;i < length;i++) {
        if (output[i] != '+') {
            return 0;
        }
    }
    return 1;
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

/// WORD-LIST COMPARATOR
// 1 = printable
// 0 = not_printable
int list_comparator(list_punt L, const char string[line_length], int length) {
    while (L != NULL) {
        if (L->valid_in_game == 1) {
            for (int i = 0;i < length;i++) {
                if (L->valid_in_game == 1) {
                    // searching for +
                    if (L->output[i] == '+') {
                        if (L->word[i] != string[i]) {
                            return 0;
                        }
                    }
                        // searching for / or |
                    else if (L->output[i] == '/' || L->output[i] == '|') {
                        if (L->word[i] == string[i]) {
                            return 0;
                        }
                    }
                }
            }
            if (L->valid_in_game == 1) {
                for (int i = 0;i < length;i++) {
                    if (L->output[i] == '|') {
                        int count_string = 0;
                        int count_list = 0;
                        for (int j = 0;j < length;j++) {
                            if (string[j] == L->word[i]) {
                                count_string++;
                            }
                            if (L->word[j] == L->word[i]) {
                                if (L->output[j] == '|' || L->output[j] == '+') {
                                    count_list++;
                                }
                            }
                        }
                        if (count_string < count_list) {
                            return 0;
                        }
                    }
                    else if (L->output[i] == '/') {
                        int count_string = 0;
                        int count_list = 0;
                        for (int j = 0; j < length; j++) {
                            if (string[j] == L->word[i]) {
                                count_string++;
                            }
                            if (L->word[j] == L->word[i]) {
                                if (j < i) {
                                    if (L->output[j] == '|') {
                                        count_list++;
                                    }
                                }
                                if (L->output[j] == '+') {
                                    count_list++;
                                }
                            }
                        }
                        if (count_list != count_string) {
                            return 0;
                        }
                    }
                }
            }
            L = L->next;
        }
    }
    return 1;
}

/// WORD-TREE COMPARATOR
int tree_comparator(tree_punt T, char word[line_length], char output[line_length], int length) {
    int number = 0;
    if (T != NULL) {
        if (T->sx != NULL) {
            number = number + tree_comparator(T->sx,word,output,length);
        }
        if (T->printable == 1) {
            for (int i = 0;i < length;i++) {
                if (T->printable == 1) {
                    if (output[i] == '+') {
                        if (word[i] != T->word[i]) {
                            number++;
                            T->printable = 0;
                        }
                    }
                    else if (output[i] == '/') {
                        if (word[i] == T->word[i]) {
                            number++;
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
                                number++;
                                T->printable = 0;
                            }
                        }
                    }
                    else if (output[i] == '|') {
                        if (word[i] == T->word[i]) {
                            number++;
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
                                number++;
                                T->printable = 0;
                            }
                        }
                    }
                }
            }
        }
        if (T->dx != NULL) {
            number = number + tree_comparator(T->dx,word,output,length);
        }
    }
    return number;
}

/// MAIN CODE
int main() {
    // local variables
    int string_length;
    int mode = 0;   // MODE: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries
    int correct;
    int comparison;
    int presence = 0;
    int max_attempts = 0;
    int actual_attempts = 0;
    int list_construct = 1;
    int initial_numbers = 0;
    int insertion_numbers = 0;
    int total_numbers = 0;
    int counter_actual_array = 0;
    int counter_total_array = 0;
    char key_word[line_length];
    char try_output[line_length];
    char string_placeholder[line_length];
    list_punt list = NULL;
    tree_punt tree = NULL;
    //pointer_punt total_pointer = NULL;
    //pointer_punt actual_pointer = NULL;

    // string_length reader
    string_length = parsing(fgets(string_placeholder,line_length,stdin));
    //printf("%d",string_length);   // testing
    pointer_punt actual_array = malloc(sizeof(struct Pointer)*2000);
    pointer_punt total_array = malloc(sizeof(struct Pointer)*100000);

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
                    //tree_reset(tree);
                    list_reset(list);
                    for (int i = 0;i < counter_total_array;i++) {
                        if (total_array[i].node->printable == 0) {
                            total_array[i].node->printable = 1;
                        }
                    }
                    counter_actual_array = 0;
                    list_construct = 1;
                    //actual_pointer = total_pointer;
                    total_numbers = initial_numbers + insertion_numbers;
                    mode = 2;
                }
            }
        }

            // word
        else {   //mode: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries
            // initial dictionary
            if (mode == 0) {
                ///tree_insert(tree,string_placeholder,1);
                if (tree == NULL) {
                    tree = new_node_tree(string_placeholder,1);
                    total_array[counter_total_array].node = tree;
                    counter_total_array++;
                }
                else {
                    tree_punt root = tree;
                    while (tree != NULL) {
                        if (custom_string_compare(string_placeholder,tree->word) < 0) {
                            if (tree->sx != NULL) {
                                tree = tree->sx;
                            }
                            else {
                                tree->sx = new_node_tree(string_placeholder,1);
                                total_array[counter_total_array].node = tree->sx;
                                counter_total_array++;
                                tree = root;
                                break;
                            }
                        }
                        else {
                            if (tree->dx != NULL) {
                                tree = tree->dx;
                            }
                            else {
                                tree->dx = new_node_tree(string_placeholder,1);
                                total_array[counter_total_array].node = tree->dx;
                                counter_total_array++;
                                tree = root;
                                break;
                            }
                        }
                    }
                }
                initial_numbers++;
            }
                // insertions
            else if (mode == 1) {
                if (list_construct == 1) {
                    /// tree_insert(tree,string_placeholder,comparison);
                    tree_punt root = tree;
                    while (tree != NULL) {
                        if (custom_string_compare(string_placeholder,tree->word) < 0) {
                            if (tree->sx != NULL) {
                                tree = tree->sx;
                            }
                            else {
                                tree->sx = new_node_tree(string_placeholder,1);
                                total_array[counter_total_array].node = tree->sx;
                                counter_total_array++;
                                tree = root;
                                break;
                            }
                        }
                        else {
                            if (tree->dx != NULL) {
                                tree = tree->dx;
                            }
                            else {
                                tree->dx = new_node_tree(string_placeholder,1);
                                total_array[counter_total_array].node = tree->dx;
                                counter_total_array++;
                                tree = root;
                                break;
                            }
                        }
                    }
                    total_numbers++;
                    insertion_numbers++;
                }
                else {
                    comparison = list_comparator(list,string_placeholder,string_length);
                    ///tree_insert(tree,string_placeholder,comparison);
                    tree_punt root = tree;
                    while (tree != NULL) {
                        if (custom_string_compare(string_placeholder,tree->word) < 0) {
                            if (tree->sx != NULL) {
                                tree = tree->sx;
                            }
                            else {
                                tree->sx = new_node_tree(string_placeholder,comparison);
                                total_array[counter_total_array].node = tree->sx;
                                counter_total_array++;
                                if (comparison == 1) {
                                    ///actual_pointer = pointer_insert(actual_pointer,tree->sx);
                                    actual_array[counter_actual_array].node = tree->sx;
                                    counter_actual_array++;
                                }
                                tree = root;
                                break;
                            }
                        }
                        else {
                            if (tree->dx != NULL) {
                                tree = tree->dx;
                            }
                            else {
                                tree->dx = new_node_tree(string_placeholder,comparison);
                                total_array[counter_total_array].node = tree->dx;
                                counter_total_array++;
                                if (comparison == 1) {
                                    ///actual_pointer = pointer_insert(actual_pointer,tree->dx);
                                    actual_array[counter_actual_array].node = tree->dx;
                                    counter_actual_array++;
                                }
                                tree = root;
                                break;
                            }
                        }
                    }
                    if (comparison == 1) {
                        total_numbers++;
                    }
                    insertion_numbers++;
                }
            }
                // tries
            else {
                // searching if the word is present in the dictionary
                presence = 0;
                presence = tree_find_word(tree,string_placeholder);

                // playing the game motherfucker
                if (presence == 1) {
                    //printf("------------\n");   // testing
                    actual_attempts--;
                    output_generator(key_word, string_placeholder, try_output, string_length);
                    correct = check_try(try_output, string_length);
                    if (correct == 1) {
                        fputs("ok\n",stdout);
                    }
                    else {
                        fputs(try_output, stdout);
                        if (list_find_word(list, string_placeholder) == 0) {
                            list = list_insert(list, string_placeholder, try_output);
                        } else {
                            list_set_output(list, string_placeholder, try_output);
                        }
                        // printing number of remaining words in-game
                        if (list_construct == 1) {
                            ///total_numbers = total_numbers - bounds_comparator(&total_array,counter_total_array,string_placeholder, try_output,string_length);
                            int number = 0;
                            for (int i = 0;i < counter_total_array;i++) {
                                if (total_array[i].node->printable == 1) {
                                    for (int j = 0; j < string_length; j++) {
                                        if (try_output[j] == '/' || try_output[j] == '|') {
                                            if (total_array[i].node->word[j] == string_placeholder[j]) {
                                                number++;
                                                total_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                        else {
                                            if (total_array[i].node->word[j] != string_placeholder[j]) {
                                                number++;
                                                total_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (total_array[i].node->printable == 1) {
                                    for (int j = 0; j < string_length; j++) {
                                        if (total_array[i].node->printable == 1) {
                                            if (try_output[j] == '|') {
                                                int count_word = 0;
                                                int count_tree = 0;
                                                for (int k = 0; k < string_length; k++) {
                                                    if (total_array[i].node->word[k] == string_placeholder[j]) {
                                                        count_tree++;
                                                    }
                                                    if (string_placeholder[k] == string_placeholder[j]) {
                                                        if (try_output[k] == '|' || try_output[k] == '+') {
                                                            count_word++;
                                                        }
                                                    }
                                                }
                                                if (count_tree < count_word) {
                                                    number++;
                                                    total_array[i].node->printable = 0;
                                                    break;
                                                }
                                            }
                                            else if (try_output[j] == '/') {
                                                int count_word = 0;
                                                int count_tree = 0;
                                                for (int k = 0; k < string_length; k++) {
                                                    if (total_array[i].node->word[k] == string_placeholder[j]) {
                                                        count_tree++;
                                                    }
                                                    if (string_placeholder[k] == string_placeholder[j]) {
                                                        if (try_output[k] == '|' || try_output[k] == '+') {
                                                            count_word++;
                                                        }
                                                    }
                                                }
                                                if (count_tree != count_word) {
                                                    number++;
                                                    total_array[i].node->printable = 0;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            total_numbers = total_numbers - number;
                            list_construct = 0;
                            for (int i = 0;i < counter_total_array;i++) {
                                if (total_array[i].node->printable == 1) {
                                    ///actual_pointer = pointer_insert(actual_pointer,ref->node);
                                    actual_array[counter_actual_array].node = total_array[i].node;
                                    counter_actual_array++;
                                }
                                //else {
                                //    total_array[i].node->printable = 1;
                                //}
                            }
                        }
                        else {
                            ///total_numbers = total_numbers - bounds_comparator(actual_pointer,string_placeholder,try_output,string_length);
                            int number = 0;
                            for (int i = 0; i < counter_actual_array; i++) {
                                if (actual_array[i].node->printable == 1) {
                                    for (int j = 0; j < string_length; j++) {
                                        if (try_output[j] == '+') {
                                            if (actual_array[i].node->word[j] != string_placeholder[j]) {
                                                number++;
                                                actual_array[i].node->printable = 0;
                                                break;
                                            }
                                        } else if (try_output[j] == '|' || try_output[j] == '/') {
                                            if (actual_array[i].node->word[j] == string_placeholder[j]) {
                                                number++;
                                                actual_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                    }
                                    if (actual_array[i].node->printable == 1) {
                                        for (int j = 0; j < string_length; j++) {
                                            if (try_output[j] == '|') {
                                                int count_string = 0;
                                                int count_array = 0;
                                                for (int k = 0; k < string_length; k++) {
                                                    if (string_placeholder[k] == string_placeholder[j]) {
                                                        if (try_output[k] == '|' || try_output[k] == '+') {
                                                            count_string++;
                                                        }
                                                    }
                                                    if (actual_array[i].node->word[k] == string_placeholder[j]) {
                                                        count_array++;
                                                    }
                                                }
                                                if (count_array < count_string) {
                                                    number++;
                                                    actual_array[i].node->printable = 0;
                                                    break;
                                                }
                                            }
                                            else if (try_output[j] == '/') {
                                                int count_string = 0;
                                                int count_array = 0;
                                                for (int k = 0; k < string_length; k++) {
                                                    if (string_placeholder[k] == string_placeholder[j]) {
                                                        if (try_output[k] == '|' || try_output[k] == '+') {
                                                            count_string++;
                                                        }
                                                    }
                                                    if (actual_array[i].node->word[k] == string_placeholder[j]) {
                                                        count_array++;
                                                    }
                                                }
                                                if (count_array != count_string) {
                                                    number++;
                                                    actual_array[i].node->printable = 0;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            total_numbers = total_numbers - number;
                        }
                        printf("%d\n",total_numbers);
                        if (actual_attempts == 0) {
                            fputs("ko\n",stdout);
                        }
                        //printf("remaining attempts: %d\n",max_attempts);   // testing
                        // printf("------------\n");          //testing
                    }
                }
                else {
                    fputs("not_exists\n",stdout);
                    //printf("remaining attempts: %d\n",actual_attempts);   // testing
                }
            }
        }
    }
    // free memory
    list_free(list);
    //pointer_print(total_pointer,string_length);
    //pointer_free(total_pointer);
    //pointer_free(actual_pointer);
    tree_free(tree);
    free(actual_array);
    free(total_array);

/// testing
//printf("initial words = %d\n",initial_numbers);
//printf("words length = %d\n",string_length);
//printf("new words = %d\n",insertion_numbers);
//printf("total words = %d\n", total_numbers);
//printf("prints number = %d\n",print_number);
//printf("key word: %s\n",key_word);
//printf("try word: %s\n",try_word);
//printf("number of tries: %d\n",max_attempts);
}
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

/// NEW NODE CREATION
pointer_punt new_node_pointer(tree_punt T) {
    pointer_punt new
            = (pointer_punt)malloc(sizeof (struct Pointer));
    new->node = T;
    new->next =NULL;
    return new;
}

/// POINTERS LIST INSERTION
pointer_punt pointer_insert(pointer_punt P, tree_punt T) {
    pointer_punt new = new_node_pointer(T);
    new->next = P;
    return new;
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

/// DELETE ELEMENTS FROM POINTERS LIST
pointer_punt delete_element(pointer_punt P) {
    if(P != NULL) {
        pointer_punt ref = P;
        while (ref->node->printable == 0) {
            ref = P->next;
        }
        P = ref;
        while (ref->next != NULL) {
            if (ref->next->node->printable == 0) {
                ref->next = ref->next->next;
            }
            ref = ref->next;
        }

    }
    return P;
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

/// PRINT POINTERS LIST
void pointer_print(pointer_punt P, int length) {
    if (P != NULL) {
        for (int i=0;i <= length;i++) {
            putchar(P->node->word[i]);
        }
        pointer_print(P->next,length);
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

/// DELETE ELEMENT INTO THE POINTERS LIST
pointer_punt delete_head_node(pointer_punt head) {
    pointer_punt new_head = head->next;
    head->next = NULL;
    free(head);
    return new_head;
}

//TODO: delete elements with printable == 0
void delete_node(pointer_punt P) {
    if (P != NULL) {

    }
}

/// FIND WORD INTO THE TREE
// 1 = yes
// 0 = no
int tree_find_word(tree_punt T, char string[line_length]) {
    int check;
    while (T != NULL) {
        check = strcmp(string,T->word);
        if ( check == 0) {
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
        if (strcmp(string,L->word) == 0) {
            return 1;
        }
        L = L->next;
    }
    return 0;
}

/// LIST OUTPUT SETTER
void list_set_output(list_punt L, char string[line_length], char output[line_length]) {
    while (L != NULL) {
        if (strcmp(string,L->word) == 0) {
            strcpy(L->output,output);
            L->valid_in_game = 1;
            return;
        }
        L = L->next;
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

/// BOUNDS COMPARATOR
int bounds_comparator(pointer_punt P, const char word[line_length], const char output[line_length], int length) {
    int number = 0;
    while (P != NULL) {
        if (P->node->printable == 1) {
            for (int i = 0; i < length; i++) {
                if (output[i] == '/' || output[i] == '|') {
                    if (P->node->word[i] == word[i]) {
                        number++;
                        P->node->printable = 0;
                        break;
                    }
                }
                else {
                    if (P->node->word[i] != word[i]) {
                        number++;
                        P->node->printable = 0;
                        break;
                    }
                }
            }
        }
        if (P->node->printable == 1) {
            for (int i = 0; i < length; i++) {
                if (P->node->printable == 1) {
                    if (output[i] == '|') {
                        int count_word = 0;
                        int count_tree = 0;
                        for (int j = 0; j < length; j++) {
                            if (P->node->word[j] == word[i]) {
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
                            P->node->printable = 0;
                            break;
                        }
                    } else if (output[i] == '/') {
                        int count_word = 0;
                        int count_tree = 0;
                        for (int j = 0; j < length; j++) {
                            if (P->node->word[j] == word[i]) {
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
                            P->node->printable = 0;
                            break;
                        }
                    }
                }
            }
        }
        P = P->next;
    }
    return number;
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

/// MAIN CODE
int main() {

    // local variables
    int string_length;
    int mode = 0;   // MODE: 0 = initial_mode (initial dictionary), 1 = insertions, 2 = game tries
    int correct;
    int comparison;
    int presence = 0;
    int max_attempts;
    int actual_attempts;
    int initial_numbers = 0;
    int insertion_numbers = 0;
    int total_numbers = 0;
    char key_word[line_length];
    char try_output[line_length];
    char string_placeholder[line_length];
    list_punt list = NULL;
    tree_punt tree = NULL;
    pointer_punt total_pointer = NULL;
    pointer_punt actual_pointer = NULL;

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
                    actual_pointer = NULL;
                    actual_pointer = total_pointer;
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
                    total_pointer = pointer_insert(total_pointer,tree);
                }
                else {
                    tree_punt root = tree;
                    while (tree != NULL) {
                        if (strcmp(string_placeholder,tree->word) < 0) {
                            if (tree->sx != NULL) {
                                tree = tree->sx;
                            }
                            else {
                                tree->sx = new_node_tree(string_placeholder,1);
                                total_pointer = pointer_insert(total_pointer,tree->sx);
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
                                total_pointer = pointer_insert(total_pointer,tree->dx);
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
                comparison = list_comparator(list,string_placeholder,string_length);
                ///tree_insert(tree,string_placeholder,comparison);
                tree_punt root = tree;
                while (tree != NULL) {
                    if (strcmp(string_placeholder,tree->word) < 0) {
                        if (tree->sx != NULL) {
                            tree = tree->sx;
                        }
                        else {
                            tree->sx = new_node_tree(string_placeholder,comparison);
                            total_pointer = pointer_insert(total_pointer,tree->sx);
                            if (comparison == 1) {
                                actual_pointer = pointer_insert(actual_pointer,tree->sx);
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
                            total_pointer = pointer_insert(total_pointer,tree->dx);
                            if (comparison == 1) {
                                actual_pointer = pointer_insert(actual_pointer,tree->dx);
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
                // tries
            else {
                // searching if the word is present in the dictionary
                presence = 0;
                presence = tree_find_word(tree,string_placeholder);

                // playing the game motherfucker
                if (presence == 1) {
                    //printf("------------\n");   // testing
                    actual_attempts--;
                    output_generator(key_word,string_placeholder,try_output,string_length);
                    correct = check_try(try_output,string_length);
                    if (correct == 1) {
                        puts("ok");
                    }
                    else {
                        for (int i = 0; i <= string_length;i++) {
                            putchar_unlocked(try_output[i]);
                        }
                        if (list_find_word(list,string_placeholder) == 0) {
                            list = list_insert(list,string_placeholder,try_output);
                        }
                        else {
                            list_set_output(list,string_placeholder,try_output);
                        }
                        // printing number of remaining words in-game
                        total_numbers = total_numbers - bounds_comparator(actual_pointer,string_placeholder,try_output,string_length);
                        printf("%d\n",total_numbers);
                        //TODO: eliminare elementi che puntano a nodi con printable = 0
                        //actual_pointer = delete_element(actual_pointer);
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
    list_free(list);
    //pointer_print(total_pointer,string_length);
    pointer_free(total_pointer);
    //pointer_free(actual_pointer);
    tree_free(tree);

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
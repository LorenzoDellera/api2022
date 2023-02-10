#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define line_length 30

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

/// POINTERS LIST STRUCT
typedef struct Pointer *pointer_punt;
struct Pointer {
    tree_punt node;
};

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

/// CUSTOM INT PRINT
void print_fast(int value) {
    if (value / 10) {
        print_fast(value / 10);
    }
    putchar((int) value % 10 + '0');
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

/// PRINT TREE
void tree_print(tree_punt T) {
    if (T != NULL) {
        tree_print(T->sx);
        if (T->printable == 1) {
            fputs(T->word,stdout);
        }
        tree_print(T->dx);
    }
}

/// PRINT TREE
void uncondition_tree_print(tree_punt T) {
    if (T != NULL) {
        tree_print(T->sx);
        fputs(T->word,stdout);
        tree_print(T->dx);
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
    tree_punt tree = NULL;

    // string_length reader
    //string_length = parsing(fgets(string_placeholder,line_length,stdin));
    if (scanf("%d",&string_length)) {}
    if (fgets(string_placeholder,line_length,stdin)) {}
    //printf("%d",string_length);   // testing

    // matrix
    int occurrences[64][2];
    int min_number[string_length];
    int right_number[string_length];
    char matrix[64][string_length+1];
    matrix[0][0] = '-';
    matrix[1][0] = '0';
    matrix[2][0] = '1';
    matrix[3][0] = '2';
    matrix[4][0] = '3';
    matrix[5][0] = '4';
    matrix[6][0] = '5';
    matrix[7][0] = '6';
    matrix[8][0] = '7';
    matrix[9][0] = '8';
    matrix[10][0] = '9';
    matrix[11][0] = 'A';
    matrix[12][0] = 'B';
    matrix[13][0] = 'C';
    matrix[14][0] = 'D';
    matrix[15][0] = 'E';
    matrix[16][0] = 'F';
    matrix[17][0] = 'G';
    matrix[18][0] = 'H';
    matrix[19][0] = 'I';
    matrix[20][0] = 'J';
    matrix[21][0] = 'K';
    matrix[22][0] = 'L';
    matrix[23][0] = 'M';
    matrix[24][0] = 'N';
    matrix[25][0] = 'O';
    matrix[26][0] = 'P';
    matrix[27][0] = 'Q';
    matrix[28][0] = 'R';
    matrix[29][0] = 'S';
    matrix[30][0] = 'T';
    matrix[31][0] = 'U';
    matrix[32][0] = 'V';
    matrix[33][0] = 'W';
    matrix[34][0] = 'X';
    matrix[35][0] = 'Y';
    matrix[36][0] = 'Z';
    matrix[37][0] = '_';
    matrix[38][0] = 'a';
    matrix[39][0] = 'b';
    matrix[40][0] = 'c';
    matrix[41][0] = 'd';
    matrix[42][0] = 'e';
    matrix[43][0] = 'f';
    matrix[44][0] = 'g';
    matrix[45][0] = 'h';
    matrix[46][0] = 'i';
    matrix[47][0] = 'j';
    matrix[48][0] = 'k';
    matrix[49][0] = 'l';
    matrix[50][0] = 'm';
    matrix[51][0] = 'n';
    matrix[52][0] = 'o';
    matrix[53][0] = 'p';
    matrix[54][0] = 'q';
    matrix[55][0] = 'r';
    matrix[56][0] = 's';
    matrix[57][0] = 't';
    matrix[58][0] = 'u';
    matrix[59][0] = 'v';
    matrix[60][0] = 'w';
    matrix[61][0] = 'x';
    matrix[62][0] = 'y';
    matrix[63][0] = 'z';

    // pointers lists
    pointer_punt actual_array = malloc(sizeof(struct Pointer)*10000000);
    pointer_punt total_array = malloc(sizeof(struct Pointer)*10000000);

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
                if (list_construct == 1) {
                    uncondition_tree_print(tree);
                }
                else {
                    tree_print(tree);
                }
                //printf("------------\n");   // testing
                //print_number++;   // testing
                mode = 2;
            }
                // +new_game
            else if (string_placeholder[1] == 'n') {
                if (fgets(key_word, line_length, stdin) != NULL) {
                    //max_attempts = parsing(fgets(string_placeholder,line_length,stdin));
                    if (scanf("%d",&max_attempts)) {}
                    if (fgets(string_placeholder,line_length,stdin)) {}
                    actual_attempts = max_attempts;
                    // matrix reset
                    for (int i = 0;i < 32;i++) {
                        for (int j = 1;j < string_length+1;j++) {
                            matrix[i][j] = 'n';
                            matrix[63-i][j] = 'n';
                        }
                        occurrences[i][0] = -1;
                        occurrences[i][1] = -1;
                        occurrences[63-i][0] = -1;
                        occurrences[63-i][1] = -1;
                    }
                    // tree reset
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
                    comparison = 1;
                    // matrix comparison
                    for (int i = 0;i < 32;i++) {
                        if (comparison == 0) {
                            break;
                        }
                        if (occurrences[i][0] != -1 || occurrences[i][1] != -1) {
                            if (occurrences[i][0] != -1) {
                                int occ = 0;
                                for (int j = 0;j < string_length;j++) {
                                    if (matrix[i][0] == string_placeholder[j] && matrix[i][j+1] != '/') {
                                        occ++;
                                    }
                                }
                                if (occ != occurrences[i][0]) {
                                    comparison = 0;
                                    break;
                                }
                            }
                            else if (occurrences[i][1] != -1) {
                                    int occ = 0;
                                    for (int j = 0;j < string_length;j++) {
                                        if (matrix[i][0] == string_placeholder[j] && matrix[i][j+1] != '/') {
                                            occ++;
                                        }
                                    }
                                    if (occ < occurrences[i][1]) {
                                        comparison = 0;
                                        break;
                                    }
                            }
                        }
                        if (occurrences[63-i][0] != -1 || occurrences[63-i][1] != -1) {
                            if (occurrences[63-i][0] != -1) {
                                int occ = 0;
                                for (int j = 0;j < string_length;j++) {
                                    if (matrix[63-i][0] == string_placeholder[j] && matrix[63-i][j+1] != '/') {
                                        occ++;
                                    }
                                }
                                if (occ != occurrences[63-i][0]) {
                                    comparison = 0;
                                    break;
                                }
                            }
                            else if (occurrences[63-i][1] != -1) {
                                int occ = 0;
                                for (int j = 0;j < string_length;j++) {
                                    if (matrix[63-i][0] == string_placeholder[j] && matrix[63-i][j+1] != '/') {
                                        occ++;
                                    }
                                }
                                if (occ < occurrences[63-i][1]) {
                                    comparison = 0;
                                    break;
                                }
                            }
                        }
                        for (int j = 0;j < string_length;j++) {
                            if (matrix[i][j+1] == '+') {
                                if (string_placeholder[j] != matrix[i][0]) {
                                    comparison = 0;
                                    break;
                                }
                            }
                            else if (matrix[i][0] == string_placeholder[j]) {
                                if (matrix[i][j+1] == '|' || matrix[i][j+1] == '/') {
                                    comparison = 0;
                                    break;
                                }
                            }
                            if (matrix[63-i][j+1] == '+') {
                                if (string_placeholder[j] != matrix[63-i][0]) {
                                    comparison = 0;
                                    break;
                                }
                            }
                            else if (matrix[63-i][0] == string_placeholder[j]) {
                                if (matrix[63-i][j+1] == '|' || matrix[63-i][j+1] == '/') {
                                    comparison = 0;
                                    break;
                                }
                            }
                        }
                    }
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
                        list_construct = 1;
                    }
                    else {
                        //fputs(try_output, stdout);
                        for (int i = 0; i <= string_length ;i++) {
                            putc_unlocked(try_output[i],stdout);
                        }
                        // calculating occurrences
                        for (int i = 0;i < string_length;i++) {
                            int min_occurrences = 0;
                            int exact_occurrences = -1;
                            for (int j = 0;j < string_length;j++) {
                                if (string_placeholder[i] == string_placeholder[j]) {
                                    if (try_output[j] == '+' || try_output[j] == '|') {
                                        min_occurrences++;
                                    }
                                    else {
                                        exact_occurrences = 1;
                                    }
                                }
                            }
                            min_number[i] = min_occurrences;
                            if (exact_occurrences == 1) {
                                right_number[i] = min_number[i];
                            }
                            else {
                                right_number[i] = -1;
                            }
                        }
                        // matrix bounds update
                        for (int i = 0;i < 32;i++) {
                            for (int j = 0;j < string_length;j++) {
                                if (string_placeholder[j] == matrix[i][0]) {
                                    if (matrix[i][j+1] == 'n') {
                                        matrix[i][j+1] =  try_output[j];
                                    }
                                    if (occurrences[i][0] == -1) {
                                        occurrences[i][0] = right_number[j];
                                    }
                                    if (occurrences[i][1] == -1 || occurrences[i][1] < min_number[j]) {
                                        occurrences[i][1] = min_number[j];
                                    }
                                }
                                else if (string_placeholder[j] == matrix[63-i][0]) {
                                    if (matrix[63-i][j+1] == 'n') {
                                        matrix[63-i][j+1] =  try_output[j];
                                    }
                                    if (occurrences[63-i][0] == -1) {
                                        occurrences[63-i][0] = right_number[j];
                                    }
                                    if (occurrences[63-i][1] == -1 || occurrences[63-i][1] < min_number[j]) {
                                        occurrences[63-i][1] = min_number[j];
                                    }
                                }
                            }
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
                                        if (try_output[string_length-j] == '/' || try_output[string_length-j] == '|') {
                                            if (total_array[i].node->word[string_length-j] == string_placeholder[string_length-j]) {
                                                number++;
                                                total_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                        else if (try_output[string_length-j] == '+'){
                                            if (total_array[i].node->word[string_length-j] != string_placeholder[string_length-j]) {
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
                                    actual_array[counter_actual_array] = total_array[i];
                                    counter_actual_array++;
                                }
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
                                        }
                                        else {
                                            if (actual_array[i].node->word[j] == string_placeholder[j]) {
                                                number++;
                                                actual_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                        if (try_output[string_length-j] == '|' || try_output[string_length-j] == '/') {
                                            if (actual_array[i].node->word[string_length-j] == string_placeholder[string_length-j]) {
                                                number++;
                                                actual_array[i].node->printable = 0;
                                                break;
                                            }
                                        }
                                        else if (try_output[string_length-j] == '+'){
                                            if (actual_array[i].node->word[string_length-j] != string_placeholder[string_length-j]) {
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
                        //printf("%d\n",total_numbers);
                        print_fast(total_numbers);
                        putchar('\n');
                        if (actual_attempts == 0) {
                            fputs("ko\n",stdout);
                            list_construct = 1;
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
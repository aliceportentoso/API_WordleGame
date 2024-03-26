#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define bucket_length 4
#define hash_table_length 4096 //2^12
#define total_chars 64

typedef enum {adding_to_vocabulary, inserting_reference_word, defining_attempts, guessing} State;
typedef unsigned short ASCII_index_t;
typedef unsigned short k_t;
typedef u_int16_t hash_t;
typedef char* word_t; //TODO: capire const

typedef struct vocabulary_node* vocabulary_node_pointer;
typedef const struct vocabulary_node{
    vocabulary_node_pointer next;
    char word[];
}vocabulary_node;

typedef struct overflow_node* overflow_node_pointer;
typedef struct overflow_node{
    overflow_node_pointer next;
    word_t word;
}overflow_node;

typedef struct bucket{
    word_t words_pointers[bucket_length];
    overflow_node* overflow_list;
}bucket;

word_t add_to_vocabulary_list();
int8_t my_strcmp(word_t word1, word_t word2);
void stampa_filtrate(bool print_words);
void add_to_vocabulary_hash_table(word_t word);
hash_t hash_func(word_t word);
ASCII_index_t char_to_index(char c);
void new_guess();

k_t k;
State state;
vocabulary_node_pointer* express_list;
/*vocabulary_node_pointer vocabulary_list;*/
bucket* vocabulary_hash_table;

word_t reference_word;
unsigned attempts;
k_t guess_char_counter[total_chars], reference_char_counter[total_chars];
k_t found_char_counter[total_chars], current_char_counter[total_chars];
bool complete_char_registry[total_chars];
bool** char_matrix;

int main() {
    char c;
    state = adding_to_vocabulary;
    State prev_state = adding_to_vocabulary;

    //lettura di k
    c = getchar();
    k = c - '0';
    while ((c = getchar()) != '\n')
        k = k * 10 + c - '0';

    //inizializzazioni
    if(!(express_list = malloc(sizeof(vocabulary_node) * total_chars)))
        exit(1);
    if(!(vocabulary_hash_table = malloc(sizeof(bucket) * hash_table_length)))
        exit(1);
    if(!(reference_word = malloc(sizeof(char) * k)))
        exit(1);
    if(!(char_matrix = malloc(sizeof(bool*) * k)))
        exit(1);
    for (k_t i = 0; i < k; i++)
        if(!(char_matrix[i] = malloc(sizeof(bool) * total_chars)))
            exit(1);

    //programma
    while ((c = getchar()) != EOF) {
        if (c == '+') {
            switch (getchar()) {
                case 'n':
                    for (ASCII_index_t i = 0; i < total_chars; i++) {
                        reference_char_counter[i] = 0;
                        found_char_counter[i] = 0;
                        complete_char_registry[i] = false;
                    }
                    for (k_t i = 0; i < k; i++)
                        for (ASCII_index_t j = 0; j < total_chars; j++)
                            char_matrix[i][j] = true;
                    state = inserting_reference_word;
                    break;

                case 's':
                    stampa_filtrate(true);
                    break;

                case 'i': //i(nserisci_)
                    while (getchar() != '_'); //inserisci_
                    if (getchar() == 'i') {
                        prev_state = state;
                        state = adding_to_vocabulary; //inserisci_i
                    } else
                        state = prev_state;
                    break;
            }
            while (getchar() != '\n');

        } else {
            switch (state) {
                case adding_to_vocabulary:
                    ungetc(c, stdin);
                    add_to_vocabulary_hash_table(add_to_vocabulary_list());
                    break;

                case inserting_reference_word:
                    reference_word[0] = c;
                    for (k_t i = 1; i < k; ++i)
                        reference_word[i] = getchar();

                    for (k_t i = 0; i < k; i++)
                        reference_char_counter[char_to_index(reference_word[i])]++;

                    state = defining_attempts;
                    break;

                case defining_attempts:
                    attempts = c - '0';
                    while ((c = getchar()) != '\n')
                        attempts = attempts * 10 + c - '0';
                    ungetc(c, stdin); //restituisce '\n'
                    state = guessing;
                    break;

                case guessing:
                    ungetc(c, stdin);
                    new_guess();
                    if (!attempts)
                        state = adding_to_vocabulary;
                    break;
            }
            getchar(); //consuma '\n'
        }
    }
}

int8_t my_strcmp(word_t word1, word_t word2) {
    for (k_t i = 0; i < k; i++)
        if (word1[i] != word2[i])
            return word1[i] < word2[i] ? -1 : 1;
    return 0;
}

bool is_not_in_dictionary(const word_t word) {
    const hash_t hash = hash_func(word);

    for (unsigned short i = 0; i < bucket_length; i++) {
        if (!vocabulary_hash_table[hash].words_pointers[i])
            return true;
        if (!my_strcmp(vocabulary_hash_table[hash].words_pointers[i], word))
            return false;
    }
    for (overflow_node_pointer curr = vocabulary_hash_table[hash].overflow_list; curr; curr = curr->next)
        if (!my_strcmp(curr->word, word))
            return false;

    return true;
}

void new_guess() {
    char guess[k];
    for (k_t i = 0; i < k; i++)
        guess[i] = getchar();

    if (!my_strcmp(reference_word, guess)) {
        puts("ok");
        attempts = 0;
        return;
    }
    if (is_not_in_dictionary(guess)) {
        puts("not_exists");
        return;
    }

    ASCII_index_t guess_indexes[k];
    for (k_t i = 0; i < k; i++)
        guess_indexes[i] = char_to_index(guess[i]);
    for (ASCII_index_t i = 0; i < total_chars; i++) {
        guess_char_counter[i] = reference_char_counter[i];
    }

    char result[k];

    for (k_t i = 0; i < k; i++) {
        if (guess[i] == reference_word[i]) {
            result[i] = '+';

            for (ASCII_index_t j = 0; j < total_chars; j++)
                if (j != guess_indexes[i])
                    char_matrix[i][j] = false;
            guess_char_counter[guess_indexes[i]]--;
        } else {
            result[i] = '/';

            char_matrix[i][guess_indexes[i]] = false;
        }
    }
    for (k_t i = 0; i < k; i++) {
        if (result[i] != '/')
            continue;

        if (guess_char_counter[guess_indexes[i]] > 0) { //stampa '|'
            result[i] = '|';
            guess_char_counter[guess_indexes[i]]--;
        } else //stampa '/'
            complete_char_registry[guess_indexes[i]] = true;
    }

    for (ASCII_index_t i = 0; i < total_chars; i++)
        if (reference_char_counter[i] - guess_char_counter[i] > found_char_counter[i])
            found_char_counter[i] = reference_char_counter[i] - guess_char_counter[i];

    for (k_t i = 0; i < k; i++)
        putchar(result[i]);
    putchar('\n');

    stampa_filtrate(false);

    attempts--;
    if (!attempts)
        puts("ko");
}

word_t add_to_vocabulary_list() {
    //creazione nuovo nodo
    vocabulary_node_pointer new_vocabulary_node;
    if(!(new_vocabulary_node = malloc(sizeof(vocabulary_node) + k*sizeof(char))))
        exit(1);
    for(k_t i = 0; i < k; i++)
        new_vocabulary_node->word[i] = getchar();
    new_vocabulary_node->next = NULL;

    const ASCII_index_t first_index = char_to_index(new_vocabulary_node->word[0]);
    vocabulary_node_pointer vocabulary_list_p = express_list[first_index];

    //inserimento ordinato
    if (!vocabulary_list_p) //lista vuota...
        express_list[first_index] = new_vocabulary_node;
    else {//...tutti gli altri casi
        //trovo la posizione corretta...
        vocabulary_node_pointer prev = NULL, curr = vocabulary_list_p;
        for (; curr && my_strcmp(curr->word, new_vocabulary_node->word) < 0; curr = curr->next)
            prev = curr;

        if (!curr) //...fondo della lista
            prev->next = new_vocabulary_node;
        else if (curr == vocabulary_list_p) {//...testa della lista
            new_vocabulary_node->next = vocabulary_list_p;
            express_list[first_index] = new_vocabulary_node;
        } else {//...in mezzo alla lista
            prev->next = new_vocabulary_node;
            new_vocabulary_node->next = curr;
        }
    }

    return new_vocabulary_node->word;
}

void stampa_filtrate(const bool print_words) {
    int eligibles_counter = 0;

    if (state != guessing) {
        for (vocabulary_node_pointer *p = express_list; p <= &express_list[total_chars]; p++) {
            for (vocabulary_node_pointer curr = *p; curr; curr = curr->next) {
                for (k_t i = 0; i < k; i++)
                    putchar(curr->word[i]);
                putchar('\n');
            }
        }
        return;
    }

    for (vocabulary_node_pointer *p = express_list; p <= &express_list[total_chars]; p++) {
        for (vocabulary_node_pointer curr = *p; curr; curr = curr->next) {
            bool to_be_printed = true;
            for (ASCII_index_t i = 0; i < total_chars; i++)
                current_char_counter[i] = 0;

            for (k_t i = 0; i < k; i++) {
                const ASCII_index_t index = char_to_index(curr->word[i]);
                if (!char_matrix[i][index]) {
                    to_be_printed = false;
                    break;
                }
                current_char_counter[index]++;
            }
            if (!to_be_printed)
                continue;

            for (ASCII_index_t i = 0; i < total_chars; i++) {
                if (complete_char_registry[i] ? current_char_counter[i] != found_char_counter[i] : current_char_counter[i] < found_char_counter[i]) {
                    to_be_printed = false;
                    break;
                }
            }
            if (!to_be_printed)
                continue;

            if (print_words) {
                for (k_t i = 0; i < k; i++)
                    putchar(curr->word[i]);
                putchar('\n');
            } else
                eligibles_counter++;
        }
    }

    if (!print_words)
        printf("%d\n", eligibles_counter);
}

hash_t hash_func(const word_t word) {
    unsigned long hash = 5381;

    for (k_t i = 0; i < k; i++)
        hash = ((hash << 5) + hash) + word[i];

    return hash % hash_table_length;
}

void add_to_vocabulary_hash_table(const word_t word) {
    int index = 0;
    const hash_t hash = hash_func(word);
    for (; index < bucket_length; index++)
        if (!vocabulary_hash_table[hash].words_pointers[index])
            break;

    if (index < bucket_length)
        vocabulary_hash_table[hash].words_pointers[index] = word;
    else { //va inserito nella lista di overflow
        //creo nuovo nodo
        overflow_node_pointer new_overflow_node;
        if (!(new_overflow_node = malloc(sizeof(overflow_node))))
            exit(1);
        new_overflow_node->word = word;
        //inserisco nuovo nodo
        if (vocabulary_hash_table[hash].overflow_list)
            new_overflow_node->next = vocabulary_hash_table[hash].overflow_list;
        vocabulary_hash_table[hash].overflow_list = new_overflow_node;
    }
}

ASCII_index_t char_to_index(const char c){
    ASCII_index_t ascii_index = (ASCII_index_t)c;

    switch (c) {
        case 45: return 0; //0 (-)
        case 48 ... 57: return ascii_index - 47; //1,10 (numeri)
        case 65 ... 90: return ascii_index - 54; //11,36 (maiusc)
        case 95: return 37; //37 (_)
        default: return ascii_index - 59; //38,63 (minusc)
    }
}

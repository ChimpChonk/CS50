// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 35;

// Hash table
node *table[N];

// vairables
unsigned int hash_index = 0;
unsigned int word_count = 0;
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // get hash value
    hash_index = hash(word);

    // loop trough hash table to find the word
    for (node *cursor = table[hash_index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) != 0)
        {
            break;
        }

        else
        {
            return true;
        }

    }
    return false;
}

// Hashes word to a number
// sorce: https://www.hackerearth.com/practice/data-structures/hash-tables/basics-of-hash-tables/tutorial/
unsigned int hash(const char *word)
{

    unsigned long count = 0;

    // Loop trough each letter, add them and devide by number of buckets
    // to get the hash value
    for (int i = 0; i < strlen(word); i++)
    {
        count += toupper(word[i]);
    }
    // printf("Count: %lu\n", count % N);
    return count % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    word_count = 0;

    // load from file to memeory
    FILE *word_file = fopen(dictionary, "r");

    if (word_file == NULL)
    {
        printf("file could not load");
        loaded = false;
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(word_file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        hash_index = hash(word);

        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        n->next = table[hash_index];
        table[hash_index] = n;
        word_count++;
    }

    loaded = true;
    fclose(word_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // free memory
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}

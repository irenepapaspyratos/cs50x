// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5500; //1000

// Number of loaded words of dictionary
unsigned long WORD_COUNT;

// Prototypes
void ini_table(void);
bool check_table(const char *word, unsigned int h);

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash of word
    unsigned int h = hash(word);
    bool result = check_table(word, h);

    return result;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Create hash by adding the lower-case values
    unsigned int x = 0;
    for (int w = 0; w < strlen(word); w++)
    {
        char wo = tolower(word[w]);
        x += (unsigned int)wo;
    }

    return x;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Set loaded words to 0 and the value of every array-element of table also
    WORD_COUNT = 0;
    ini_table();

    // Open dictionary to load
    FILE *dict = fopen(dictionary, "r");
    if (dict != NULL)
    {
        char word[LENGTH + 1];
        while (fscanf(dict, "%s", word) == 1)
        {
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                printf("Load dictionary: n = malloc failed.");
                return false;
            }

            // Fill value of word in node and set pointer->next to NULL
            strcpy(n->word, word);
            n->next = NULL;

            // Insert Node to hash table
            unsigned int h = hash(word);
            if (table[h] != NULL)
            {
                n->next = table[h];
                table[h] = n;
                WORD_COUNT++;
            }
            else
            {
                table[h] = n;
                WORD_COUNT++;
            }
        }
    }
    else
    {
        printf("Open dictionary failed.");
        return false;
    }
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return WORD_COUNT;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Free every list element of each array element of table
    for (int n = 0; n < N; n++)
    {
        while (table[n] != NULL)
        {
            node *temp = table[n]->next;
            free(table[n]);
            table[n] = temp;
        }
    }

    return true;

}

void ini_table(void)
{
    for (int n = 0; n < N; n++)
    {
        table[n] = NULL;
    }
}

bool check_table(const char *word, unsigned int h)
{
    for (node *temp = table[h]; temp != NULL; temp = temp->next)
    {
        if (strcasecmp(temp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

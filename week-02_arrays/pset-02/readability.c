/*
letter              = a-z && A-Z
word's separation   = space
sentence's end      = . | ! | ?
output              = Grade X [X = rounded index-integer | if X>15 -> 16+ | if X<1 -> Before Grade 1]
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int *fn_count(string text);
float fn_cl_index(int *counts);

// Get rounded Coleman-Liau index for user's text
int main(void)
{
    // Get text from user
    string text = get_string("Text: ");

    // Build array of integers for letters, words, sentences
    int *counts = fn_count(text);

    // Calculate rounded Coleman-Liau index
    int index_r = round(fn_cl_index(counts));

    // Building cases of output and print
    string str_grade = "Grade ";
    if (index_r < 1 || index_r > 15)
    {
        str_grade = (index_r < 1) ? "Before Grade 1" : "Grade 16+";
        printf("%s\n", str_grade);
    }
    else
    {
        printf("%s%i\n", str_grade, index_r);
    }
}



float fn_cl_index(int *array)
{
    int letters = array[0];
    int words = array[1];
    int sents = array[2];

    // Using the Coleman-Liau index [average letters|sentences per 100 words]
    float ref = words / 100.0;
    float l = letters / ref;
    float s = sents / ref;

    float index = 0.0588 * l - 0.296 * s - 15.8;

    return index;
}


// Returning pointer to array that should be returned but can't in C
int *fn_count(string text)
{
    // Count number of letters, words, sentences
    int length = strlen(text);
    int words = 0, letters = 0, sents = 0;
    static int results[3];

    for (int i = 0; i < length; i++)
    {
        char c = text[i];
        if (isalpha(c) != 0)
        {
            letters++;
        }
        else if (isspace(c) != 0)
        {
            words++;
        }
        else if (c == '.' || c == '!' || c == '?')
        {
            sents++;
        }
    }

    // Consider wrong ends of text
    if (text[length - 1] != '.' && text[length - 1] != '!' && text[length - 1] != '?' &&
        text[length - 2] != '.' && text[length - 2] != '!' && text[length - 2] != '?')
    {
        sents++;
    }
    if (text[length - 1] != ' ')
    {
        words++;
    }

    results[0] = letters;
    results[1] = words;
    results[2] = sents;

    return results;
}

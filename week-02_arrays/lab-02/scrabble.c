#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Determine the winner
    string result;
    if (score1 < score2)
    {
        result = "Player 2 wins!";
    }
    else if (score1 > score2)
    {
        result = "Player 1 wins!";
    }
    else
    {
        result = "Tie!";
    }

    // Print the winner
    printf("%s\n", result);

}

int compute_score(string word)
{
    // Compute and return score for string

    int length = 0;
    int score = 0;

    // Calculate word's length
    do
    {
        length++;
    }
    while (word[length] != 0);

    // Calculate word's score
    score = 0;
    for (int i = 0; i < length; i++)
    {
        // Get Ascii of forced lower letter
        int ascii = 0;
        ascii = (islower(word[i]) != 0) ? word[i] : word[i] + 32;

        // Add score of letters
        score = (ascii > 96 && ascii < 123) ? score + POINTS[ascii - 97] : score;
    }

    return score;
}

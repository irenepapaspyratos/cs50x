#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Check if right name chosen
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            // Update count of candidate
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    string winner;

    // Sort Candidates votes if more than 1 candidate
    candidate tempCand[1];
    int c = candidate_count - 1;
    int touch;

    do
    {
        touch = 0;

        // Do recursive Bubble Search
        for (int i = 0; i < c; i++)
        {
            if (candidates[i].votes < candidates[i + 1].votes)
            {
                tempCand[0] = candidates[i + 1];
                candidates[i + 1] = candidates[i];
                candidates[i] = tempCand[0];

                touch++;
            }
        }
    }
    while (touch != 0);

    //Calculate Winner
    int winNo = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[0].votes == candidates[i].votes)
        {
            winNo++;
        }
    }

    for (int i = 0; i < winNo; i++)
    {
        winner = candidates[i].name;
        printf("%s\n", winner);
    }

    return;
}

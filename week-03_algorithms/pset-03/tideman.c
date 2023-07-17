#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int fn_checkpathup(int winner_a, int winner_b, int p);
int fn_checkpathdown(int winner_a, int winner_b, int p);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Check if name is valid
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // Update ranks array
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Find rank for each candidate and set self-preference of current candidate to 0
    for (int r = 0; r < candidate_count; r++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (ranks[r] == i)
            {
                if (r == 0) // Update preferances[] for each other candidate if current is ranked as first
                {
                    for (int j = 0; j < candidate_count; j++)
                    {
                        preferences[i][j] += 1;
                    }
                    preferences[i][i] = 0;
                }
                else // Update preferances[] for each other candidate if current is not ranked as first
                {
                    // Candidates, who are NOT preferred over the current
                    for (int rcheck = r + 1; rcheck < candidate_count; rcheck++)
                    {
                        for (int j = 0; j < candidate_count; j++)
                        {
                            if (ranks[rcheck] == j)
                            {
                                preferences[i][j] += 1;
                            }
                        }
                    }
                    preferences[i][i] = 0;
                }

                break;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Calculate number of possible heads-ups
    int pairNo = 0;
    for (int i = 0;  i < candidate_count; i++)
    {
        int x = i + 1;
        int y = candidate_count - x;

        pairNo += y;
    }

    // Create all possible heads-ups with ratio 0:0
    int headsup[pairNo][4];
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int x = i + 1;
        int y = candidate_count - x;

        for (int j = 0; j < y; j++)
        {
            headsup[k][0] = i;
            headsup[k][1] = x;
            headsup[k][2] = 0;
            headsup[k][3] = 0;
            k++;
            x++;
        }
    }

    // Fill the ratios of the heads-ups with values of preferences
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != 0)
            {
                // Check, which name of the peir comes first in headsup[], then add 1 to relating ratio value
                if (i < j)
                {
                    for (int hu = 0; hu < pairNo; hu++)
                    {
                        if (headsup[hu][0] == i && headsup[hu][1] == j)
                        {
                            headsup[hu][2] = preferences[i][j];
                            headsup[hu][3] = preferences[j][i];

                            break;
                        }
                    }
                }
            }
        }
    }

    // Add pairs if ratio of a headsup exists and is not 1
    int pn = 0;
    for (int hu = 0; hu < pairNo; hu++)
    {

        if ((headsup[hu][2] > 0 || headsup[hu][3] > 0) && headsup[hu][2] != headsup[hu][3])
        {
            pair_count = pn + 1;
            if (headsup[hu][2] > headsup[hu][3])
            {
                pairs[pn].winner = headsup[hu][0];
                pairs[pn].loser = headsup[hu][1];
            }
            else
            {
                pairs[pn].winner = headsup[hu][1];
                pairs[pn].loser = headsup[hu][0];
            }
            pn++;
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sorting until sorted
    int touch;
    do
    {
        // Compare wins of bubble-pairing
        touch = 0;
        int x = 0, y = 0;
        for (int p = 0; p < pair_count - 1; p++)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (pairs[p].winner == i)
                {
                    for (int j = 0; j < candidate_count; j++)
                    {
                        if (pairs[p].loser == j)
                        {
                            x = preferences[i][j];
                            break;
                        }
                    }

                    break;
                }
            }

            for (int i = 0; i < candidate_count; i++)
            {
                if (pairs[p + 1].winner == i)
                {
                    for (int j = 0; j < candidate_count; j++)
                    {
                        if (pairs[p + 1].loser == j)
                        {
                            y = preferences[i][j];
                            break;
                        }
                    }

                    break;
                }
            }

            // Swap
            if (x < y)
            {
                int tempa = pairs[p + 1].winner;
                int tempb = pairs[p + 1].loser;
                pairs[p + 1].winner = pairs[p].winner;
                pairs[p + 1].loser = pairs[p].loser;
                pairs[p].winner = tempa;
                pairs[p].loser = tempb;

                touch++;
            }

        }
    }
    while (touch != 0);

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int i, j;

    // Iterate through pairs
    for (int p = 0; p < pair_count; p++)
    {
        i = pairs[p].winner;
        j = pairs[p].loser;

        // Lock first pair
        if (p == 0)
        {
            locked[i][j] = true;
        }
        else
        {
            //Check current pair, if candidates create circles - otherwise lock them
            int x = fn_checkpathup(i, j, p);
            int y = fn_checkpathdown(i, j, p);
            if (x != 1 && y != 1)
            {
                locked[i][j] = true;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner[pair_count];
    int count = 0;

    // Find the arrow FROM candidate i
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                int check = 0;
                // Check, if an arrow exists TO candidate i: print winner if not
                for (int k = 0; k < candidate_count; k++)
                {
                    if (locked[k][i] == true)
                    {
                        check = 1;
                    }
                }

                // Collect winners
                if (check == 0)
                {
                    winner[count] = i;
                    count++;
                }
            }
        }
    }

    // Print winners (but only once per candidate)
    if (count == 1)
    {
        printf("%s\n", candidates[winner[0]]);
    }
    else if (count > 1)
    {
        printf("%s\n", candidates[winner[0]]);
        for (int z = 1; z < count - 1; z++)
        {
            if (winner[z - 1] != winner[z])
            {
                printf("%s\n", candidates[winner[z]]);
            }
        }
    }

    return;
}


// Check upgoing path for circles
int fn_checkpathup(int winner_a, int winner_b, int p)
{
    int stop = 0;
    int new_cand;
    for (int z = 0; z < p; z++)
    {
        if (winner_a == pairs[z].winner)
        {
            for (int k = 0; k < candidate_count; k++)
            {
                if (locked[winner_a][k] == true)
                {
                    if (k == winner_b)
                    {
                        stop = 1;
                    }
                    else
                    {
                        new_cand = k;
                        return fn_checkpathup(new_cand, winner_b, p);
                    }
                }
            }
        }
    }

    int ret = (stop == 1) ? 1 : 0;
    return ret;
}

// Check downgoing path for circles
int fn_checkpathdown(int winner_a, int winner_b, int p)
{
    int stop = 0;
    int new_cand;
    for (int z = p; z > 0; z--)
    {
        if (winner_a == pairs[z].winner)
        {

            for (int k = 0; k < candidate_count; k++)
            {
                if (locked[k][winner_a] == true)
                {
                    if (k == winner_b)
                    {
                        stop = 1;
                    }
                    else
                    {
                        new_cand = k;
                        return fn_checkpathdown(new_cand, winner_b, p);
                    }
                }
            }
        }
    }

    int ret = (stop == 1) ? 1 : 0;
    return ret;
}

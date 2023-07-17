#include <cs50.h>
#include <stdio.h>
#include <math.h>


//Prototype
int calculate_years(int start, int end);

int main(void)
{
    // Prompt for start size
    int start, end;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    // Prompt for end size
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    // Number of years until we reach threshold
    int years  = (start == end) ? 0 : calculate_years(start, end);

    printf("Years: %i\n", years);
}


int calculate_years(int start, int end)
{
    int population = start;

    // Calculate needed years
    int n = 0;
    do
    {
        // Population growth for one year
        float incr = (population % 3 != 0) ? floor(population / 3) : population / 3;
        float decr = (population % 4 != 0) ? floor(population / 4) : population / 4;
        population = population + incr - decr;

        n++;
    }
    while (population < end);

    return n;
}

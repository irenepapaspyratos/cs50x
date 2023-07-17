/*
Digits used
Amex:   15      start: 34 | 37
Master: 16      start: 51-55 (most)
Visa:   13 | 16 start: 4

Checking
s = ∑(2 * every second digit from end [add digits instead of products if 2*x>9 !!!]) + ∑(leftover digits)
if (s % 10 == 0) valid else invalid [or: if last digit of s is 0]

Output
AMEX\n | MASTERCARD\n | VISA\n | INVALID\n
*/

#include <stdio.h>
#include <cs50.h>
#include <math.h>

// Prototypes
int get_noLenght(long number);
string validation(int length, long number);

int main(void)
{

    string type = "INVALID";

    // Get CC-Number from user, count number of digits and decide wether to check them
    int go = 0;
    int nl;
    unsigned long n;
    int iteration = 0;
    do
    {
        if (iteration != 0)
        {
            go = 0;
            break;
        }
        else
        {
            go = 1;

            // Get CC-Number
            n  = get_long("The number of your credit card: ");

            // Calculate number of digits
            nl = get_noLenght(n);

        }
        iteration++;
    }
    while (nl != 13 && nl != 15 && nl != 16);


    // Decide wether to check the digits or not
    if (go == 1)
    {
        type = validation(nl, n);
    }

    printf("%s\n", type);
}



// Calculate number of digits
int get_noLenght(long number)
{
    int length = 1;
    while (number > 9)
    {
        length++;
        number /= 10; // number = number/10;
    }
    return length;
}


string validation(int length, long number)
{
    string type = "INVALID";

    // Check sum of digits

    // for i = 0
    int sum = number % 10;
    long num = number / 10;
    // for the rest
    for (int i = 1; i < length; i++)
    {
        int ad;

        if (i % 2 != 0) // every second
        {
            ad = (num % 10) * 2;
            if (ad > 9)
            {
                ad = (ad % 10) + (ad / 10);
            }
            num /= 10;
        }
        else // the other ones
        {
            ad = num % 10;
            num /= 10;
        }

        sum += ad;
    }


    // Check wether the first digits are valid for each card type

    if (sum % 10 == 0)
    {
        switch (length)
        {
            case 13:
            {
                int first = number / pow(10, 12);
                if (first == 4)
                {
                    type = "VISA";
                    break;
                }
            }

            case 15:
            {
                int second = number / pow(10, 13);
                if (second == 34 || second == 37)
                {
                    type = "AMEX";
                    break;
                }
            }

            case 16:
            {
                int first = number / pow(10, 15);
                int second = number / pow(10, 14);
                if (second > 50 && second < 56)
                {
                    type = "MASTERCARD";
                    break;
                }
                else if (first == 4)
                {
                    type = "VISA";
                    break;
                }
            }

            default:
                type = "INVALID";
                break;

        }
    }

    return type;
}

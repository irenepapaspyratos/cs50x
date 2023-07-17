#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// Prototypes
int fn_check(string key);
string fn_output(string text, string key);


int main(int argc, string argv[])
{
    bool valid = false;

    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }

    // Check the key
    int val_id = fn_check(argv[1]);

    // Generate output
    if (val_id != 0)
    {
        printf("Usage: ./substitution KEY  |  KEY must contain 26 different alphabetical characters\n");
        return 1;
    }
    else
    {
        // Get messagetext
        string text = get_string("plaintext: ");

        // Generate ciphertext
        string output = fn_output(text, argv[1]);
        printf("ciphertext: %s\n", output);
    }
}



int fn_check(string key)
{
    int val = 0;
    int length = strlen(key);

    // Check for length
    if (length != 26)
    {
        val = 1;
        return val;
    }

    for (int i = 0; i < length; i++)
    {
        // Check for letters
        if (isalpha(key[i]) == 0)
        {
            val = 1;
            return val;
        }

        // Check for doubled letters
        for (int j = 0; j < 26; j++)
        {
            if (i != j && toupper(key[i]) == toupper(key[j]))
            {
                val = 1;
                return val;
            }
        }
    }
    return val;
}


string fn_output(string text, string key)
{
    // Generate translation vector
    int code[26][2];
    int a = 65;
    for (int i = 0; i < 26; i++)
    {
        int k = (islower(key[i]) > 0) ? key[i] - 32 : key[i];

        code[i][0] = a;
        code[i][1] = k;
        a++;
    }

    // Translate the input
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]) > 0)
        {
            for (int j = 0; j < 26; j++)
            {
                if (toupper(text[i]) == code[j][0])
                {
                    text[i] = (islower(text[i]) > 0) ? code[j][1] + 32 : code[j][1];
                    break;
                }
            }
        }
    }
    return text;
}

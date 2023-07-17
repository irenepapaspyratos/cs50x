#include <stdio.h>
#include <cs50.h>

// Prototypes
void write_space(int space);
void write_brick(int brick);


int main(void)
{
    // Get height from user
    int height;
    do
    {
        height = get_int("How high would you like your pyramide? ");
    }
    while (height < 1 || height > 8);

    // Generate printed lines
    string pause = "  ";
    for (int i = 0; i < height; i++)
    {
        int brick = i + 1;
        int space = height - brick;

        write_space(space);
        write_brick(brick);
        printf("%s", pause);
        write_brick(brick);
        printf("\n");
    }

}


// Generate printed spaces
void write_space(int space)
{
    char sp = ' ';
    for (int i = 0; i < space; i++)
    {
        printf("%c", sp);
    }
}

// Generate printed bricks
void write_brick(int brick)
{
    char br = '#';
    for (int i = 0; i < brick; i++)
    {
        printf("%c", br);
    }
}

#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Get single values of RGB
            BYTE blue = image[h][w].rgbtBlue;
            BYTE green = image[h][w].rgbtGreen;
            BYTE red = image[h][w].rgbtRed;

            // Calculate b&w - value
            int x = round((blue + green + red) / 3.0);

            // Change image-BYTEs
            image[h][w].rgbtBlue = x;
            image[h][w].rgbtGreen = x;
            image[h][w].rgbtRed = x;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Fill avariable with reflected rgbs
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int x = width - (w + 1);
            temp[h][x] = image[h][w];
        }
    }

    // Change image-rows
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Determine start/stop of blur-boxes
    int starth, stoph, startw, stopw, q;
    for (int h = 0; h < height; h++)
    {
        starth = h - 1;
        stoph = h + 2;

        if (h == 0)
        {
            starth++;
        }
        else if (h == height - 1)
        {
            stoph--;
        }

        for (int w = 0; w < width; w++)
        {
            startw = w - 1;
            stopw = w + 2;

            if (w == 0)
            {
                startw++;

                q = (h == 0 || h == height - 1) ? 4.0 : 6.0;
            }
            else if (w == width - 1)
            {
                stopw--;

                q = (h == 0 || h == height - 1) ? 4.0 : 6.0;
            }
            else
            {
                q = (h == 0 || h == height - 1) ? 6.0 : 9.0;
            }

            // Iterate through box-values and add them
            float b, g, r;
            b = g = r = 0;
            for (int i = starth; i < stoph; i++)
            {
                for (int j = startw; j < stopw; j++)
                {
                    b += image[i][j].rgbtBlue;
                    g += image[i][j].rgbtGreen;
                    r += image[i][j].rgbtRed;
                }
            }

            // Calculate average values
            b = round(b / q);
            g = round(g / q);
            r = round(r / q);

            // Change temp-BYTEs
            temp[h][w].rgbtBlue = (uint8_t) b;
            temp[h][w].rgbtGreen = (uint8_t) g;
            temp[h][w].rgbtRed = (uint8_t) r;
        }
    }

    // Change image-BYTES
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int kernelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int kernelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    RGBTRIPLE temp[height][width];

    // Determine start/stop of edge- and kernel-boxes
    int starth, stoph, startkh, stopkh, startw, stopw, startkw, stopkw;
    for (int h = 0; h < height; h++)
    {

        startkh = 0;
        stopkh = 3;

        starth = h - 1;
        stoph = h + 2;

        if (h == 0)
        {
            starth++;
            startkh++;
        }
        else if (h == height - 1)
        {
            stoph--;
            stopkh--;
        }

        for (int w = 0; w < width; w++)
        {
            startkw = 0;
            stopkw = 3;

            startw = w - 1;
            stopw = w + 2;

            if (w == 0)
            {
                startw++;
                startkw++;
            }
            else if (w == width - 1)
            {
                stopw--;
                stopkw--;
            }


            // Calculate box-values for Gx
            int bx, gx, rx;
            bx = gx = rx = 0;
            int counth = startkh;
            for (int i = starth; i < stoph; i++)
            {
                int countw = startkw;
                for (int j = startw; j < stopw; j++)
                {
                    bx += (image[i][j].rgbtBlue * kernelX[counth][countw]);
                    gx += (image[i][j].rgbtGreen * kernelX[counth][countw]);
                    rx += (image[i][j].rgbtRed * kernelX[counth][countw]);

                    countw++;
                }

                counth++;
            }

            // Calculate box-values for Gy
            int by, gy, ry;
            by = gy = ry = 0;
            counth = startkh;
            for (int i = starth; i < stoph; i++)
            {
                int countw = startkw;
                for (int j = startw; j < stopw; j++)
                {
                    by += (image[i][j].rgbtBlue * kernelY[counth][countw]);
                    gy += (image[i][j].rgbtGreen * kernelY[counth][countw]);
                    ry += (image[i][j].rgbtRed * kernelY[counth][countw]);

                    countw++;
                }

                counth++;
            }

            double b, g, r;
            b = round(sqrt(pow((double)bx, (int)2) + pow((double)by, (int)2)));
            g = round(sqrt(pow((double)gx, (int)2) + pow((double)gy, (int)2)));
            r = round(sqrt(pow((double)rx, (int)2) + pow((double)ry, (int)2)));

            // Change temp-BYTEs with max 255
            temp[h][w].rgbtBlue = (b < 255) ? b : 255;
            temp[h][w].rgbtGreen = (g < 255) ? g : 255;
            temp[h][w].rgbtRed = (r < 255) ? r : 255;
        }

    }

    // Change image-BYTEs
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }

    return;
}

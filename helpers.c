#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE px = image[i][j];

            int value = round((px.rgbtBlue + px.rgbtGreen + px.rgbtRed) / 3.0);
            px.rgbtBlue = value;
            px.rgbtGreen = value;
            px.rgbtRed = value;

            image[i][j] = px;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur_image[height][width];
    int rsum, gsum, bsum, cnt, ni, nj;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rsum = 0;
            gsum = 0;
            bsum = 0;
            cnt = 0;
            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    ni = i + r;
                    nj = j + c;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        cnt++;
                        rsum += image[ni][nj].rgbtRed;
                        gsum += image[ni][nj].rgbtGreen;
                        bsum += image[ni][nj].rgbtBlue;
                    }
                }
            }
            blur_image[i][j].rgbtRed = round(rsum / (float) cnt);
            blur_image[i][j].rgbtGreen = round(gsum / (float) cnt);
            blur_image[i][j].rgbtBlue = round(bsum / (float) cnt);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur_image[i][j];
        }
    }
    return;
}

// Limit values to the range [0, 255]
int clamp(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redX = 0, redY = 0;
            int greenX = 0, greenY = 0;
            int blueX = 0, blueY = 0;

            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    int ni = i + r;
                    int nj = j + c;
                    int weightX = Gx[r + 1][c + 1];
                    int weightY = Gy[r + 1][c + 1];

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        RGBTRIPLE px = image[ni][nj];

                        redX += weightX * px.rgbtRed;
                        redY += weightY * px.rgbtRed;

                        greenX += weightX * px.rgbtGreen;
                        greenY += weightY * px.rgbtGreen;

                        blueX += weightX * px.rgbtBlue;
                        blueY += weightY * px.rgbtBlue;
                    }
                }
            }
            int rvalue = round(sqrt(redX * redX + redY * redY));
            int gvalue = round(sqrt(greenX * greenX + greenY * greenY));
            int bvalue = round(sqrt(blueX * blueX + blueY * blueY));

            temp[i][j].rgbtRed = clamp(rvalue);
            temp[i][j].rgbtGreen = clamp(gvalue);
            temp[i][j].rgbtBlue = clamp(bvalue);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

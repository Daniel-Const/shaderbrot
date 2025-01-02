#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

const int screenWidth = 1200;
const int screenHeight = 800;

const float MANDELBROT_SCALE_X[2] = {-2.00, 0.47};
const float MANDELBROT_SCALE_Y[2] = {-1.12, 1.12};

const Color COLORS[16] = {
    (Color){66, 30, 16, 255},
    (Color){25, 7, 26, 255},
    (Color){9, 1, 47, 255},
    (Color){4, 4, 73, 255},
    (Color){0, 7, 100, 255},
    (Color){12, 44, 138, 255},
    (Color){24, 82, 177, 255},
    (Color){57, 125, 209, 255},
    (Color){134, 181, 229, 255},
    (Color){211, 236, 248, 255},
    (Color){241, 233, 191, 255},
    (Color){248, 201, 95, 255},
    (Color){255, 170, 0, 255},
    (Color){204, 128, 0, 255},
    (Color){153, 87, 0, 255},
    (Color){106, 52, 3, 255},
};

float scale(float val, const float oldRange[2], const float newRange[2])
{
    return (((val - oldRange[0]) * (newRange[1] - newRange[0])) / (oldRange[1] - oldRange[0])) + newRange[0];
}

int getMandelbrotIteration(int xPos, int yPos)
{
    float oldRangeX[2] = {0, screenWidth};
    float oldRangeY[2] = {0, screenHeight};
    float x0 = scale(xPos, oldRangeX, MANDELBROT_SCALE_X);
    float y0 = scale(yPos, oldRangeY, MANDELBROT_SCALE_Y);
    float x = 0;
    float y = 0;

    int iteration = 0;
    int maxIteration = 1000;
    while (x * x + y * y <= 2 * 2 && iteration < maxIteration)
    {
        float xTemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xTemp;
        iteration++;
    }

    return iteration;
}

int main(void)
{
    const char *shaderName = "pixelizer.fs";

    InitWindow(screenWidth, screenHeight, "demo");

    char *shaderSrc;
    char *currentShaderText;

    if (asprintf(&shaderSrc, "./shaders/%s", shaderName) < 0)
        return 1;

    if (asprintf(&currentShaderText, "Current Shader: %s", shaderName) < 0)
        return 1;

    Shader shader = LoadShader(0, shaderSrc);

    // Mandelbrot pixels
    Image mandelbrotImage = GenImageColor(screenWidth, screenHeight, WHITE);
    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            int iteration = getMandelbrotIteration(x, y);
            Color color;
            if (iteration >= 1000)
            {
                color = BLACK;
            }
            else
            {
                color = COLORS[iteration % 16];
            }
            ImageDrawPixel(&mandelbrotImage, x, y, color);
        }
    }

    Texture2D mandelbrotTexture = LoadTextureFromImage(mandelbrotImage);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    // Change pixel size for animation
    float pixelSizeMax = 5.0;
    float pixelSizeMin = 3.0;
    float pixWidth = 3.0;
    int pixelChangeDirection = 1;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        pixWidth = (pixWidth + (0.001 * pixelChangeDirection));
        if (pixWidth > pixelSizeMax)
        {
            pixWidth = pixelSizeMax;
            pixelChangeDirection *= -1;
        }
        else if (pixWidth <= pixelSizeMin)
        {
            pixWidth = pixelSizeMin;
            pixelChangeDirection *= -1;
        }

        BeginTextureMode(target);
        {
            ClearBackground(RAYWHITE);
            DrawTexture(mandelbrotTexture, 0, 0, WHITE);
        }
        EndTextureMode();

        BeginDrawing();
        {
            BeginShaderMode(shader);
            {
                // Pixelization Shader
                int pixWidthLoc = GetShaderLocation(shader, "pixelWidth");
                int pixHeightLoc = GetShaderLocation(shader, "pixelHeight");
                SetShaderValue(shader, pixWidthLoc, &pixWidth, SHADER_UNIFORM_FLOAT);
                SetShaderValue(shader, pixHeightLoc, &pixWidth, SHADER_UNIFORM_FLOAT);
                DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)target.texture.height}, (Vector2){0, 0}, WHITE);
            }
            EndShaderMode();
            // DrawText(currentShaderText, 190, 50, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    // Cleanup
    free(currentShaderText);
    free(shaderSrc);
    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}
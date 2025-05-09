#include "maze.h"
extern uint8_t scanCode;

// Loads a maze object with a sprite
Maze* loadMaze(uint16_t x, uint16_t y, xpm_map_t xpm) {

    // Allocate memory for the maze
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    if (maze == NULL) {
        printf("Error: Memory allocation for maze failed.\n");
        return NULL;  
    }

    // Initialize maze's position and load its sprite
    maze->x = x;
    maze->y = y;
    maze->mazeSprite = loadSprite(xpm);
    if (maze->mazeSprite == NULL) {
        printf("Error: Failed to load maze sprite.\n");
        free(maze);
        return NULL;
    }
    return maze;
}

// Gets the x-coordinate of the maze
uint16_t getMazeX(Maze* maze) {
    return maze ? maze->x : 0;  
}

// Sets the x-coordinate of the maze
void setMazeX(Maze* maze, uint16_t x) {
    if (maze) {
        maze->x = x;
    }
}

// Gets the y-coordinate of the maze
uint16_t getMazeY(Maze* maze) {
    return maze ? maze->y : 0; 
}

// Sets the y-coordinate of the maze
void setMazeY(Maze* maze, uint16_t y) {
    if (maze) {
        maze->y = y;
    }
}

// Draws the maze on the screen
int drawMaze(Maze* maze) {
    if (maze == NULL || maze->mazeSprite == NULL) {
        printf("Error: Invalid maze or maze sprite.\n");
        return 1; 
    }

    // Draw the maze using its sprite at its current position
    return drawSprite(maze->mazeSprite, maze->x, maze->y);
}

// Function to check if is path
int isPath(Maze* maze, uint16_t x, uint16_t y) {
    if (maze == NULL) {
        printf("Error: Maze sprite is NULL.\n");
        return 0; 
    }

    // Get the color of the pixel
    uint32_t pixelColor = getPixelColor(maze->mazeSprite, x, y); 

    // Check the color of the pixel 
    if (pixelColor == 0x000000) {
        return 1; 
    } 
    return 0;
}

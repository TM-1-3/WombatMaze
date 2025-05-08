#ifndef MAZE_H
#define MAZE_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"
#include "../io/KBC/i8042.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *mazeSprite;
} Maze;

/**
 * @brief Loads a maze object with a sprite
 * @param x The initial x-coordinate 
 * @param y The initial y-coordinate 
 * @param xpm The XPM map used to load the maze's sprite
 * @return A pointer to the Maze structure
 */
Maze* loadMaze(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Gets the x-coordinate of the maze
 * @param maze A pointer to the maze object
 * @return The x-coordinate of the maze
 */
uint16_t getMazeX(Maze* maze);


/**
 * @brief Sets the x-coordinate of the maze
 * @param maze A pointer to the maze object
 * @param x The new x-coordinate of the maze
 */
void setMazeX(Maze* maze, uint16_t x);

/**
 * @brief Gets the y-coordinate of the maze
 * @param maze A pointer to the maze object
 * @return The y-coordinate of the maze
 */
uint16_t getMazeY(Maze* maze);

/**
 * @brief Sets the y-coordinate of the maze
 * @param maze A pointer to the maze object
 * @param y The new y-coordinate of the maze
 */
void setMazeY(Maze* maze, uint16_t y);

/**
 * @brief Draws the maze on the screen
 * @param maze A pointer to the maze object 
 * @return 0 on success, non-zero otherwise
 */
int drawMaze(Maze* maze);

/**
 * @brief Checks if is a path
 * @param mazeSprite A pointer to the mazew
 * @param x The x-coordinate 
 * @param y The y-coordinate
 * @return 1 if a path, 0 if a wall (#)
 */
int isPath(Maze* maze, uint16_t x, uint16_t y);

#endif

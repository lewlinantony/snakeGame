# Retro Snake Game 

This is a simple implementation of the classic Snake game using the Raylib library in C++. The game features a snake that grows in length each time it eats food, and the game ends if the snake collides with the edges of the screen or itself.

## Features:
- Snake movement with keyboard controls (Arrow keys or WASD).
- Random food generation.
- Snake growth after eating food.
- Game Over state with score display and restart option.
- Simple graphics using Raylib.

## Requirements:
- [Raylib](https://www.raylib.com/) library.
- C++17 or later.

## Game Controls:
- **Arrow Keys** or **WASD** to control the snake.
- **Spacebar** or any direction key to restart the game after Game Over.

## Code Overview:

### Key Classes:
1. **Snake**:
   - Holds the snake's body (a deque of `Vector2` objects) and its movement direction.
   - Updates the snake’s position and handles growth after eating food.
   - Draws the snake on the screen.

2. **Food**:
   - Manages the food's position and texture.
   - Generates random food positions, ensuring it doesn't overlap the snake’s body.
   - Draws the food on the screen.

3. **Game**:
   - Contains the game logic, including collision detection, game over condition, and snake updates.
   - Manages the game state (running or game over).

### Functions:
- **EventTriggered**: Checks if the specified interval has passed to trigger an update.
- **elementInDeque**: Checks if a specific element exists in the deque.
- **checkCollisionApple**: Detects if the snake eats food.
- **checkCollisionEdges**: Detects if the snake hits the screen edges.
- **checkCollisionSnake**: Detects if the snake collides with itself.
- **GameOver**: Resets the game when a collision occurs.

### Drawing:
- The snake and food are drawn on the screen, and a border is displayed around the game area.
- On game over, the score is shown, and the player can restart the game by pressing any movement key or spacebar.

## How to Build & Run:

1. Install [Raylib](https://www.raylib.com/), if you haven’t already.
2. Clone this repository.
3. Compile the code using a C++ compiler with Raylib linked:
    ```bash
    g++ -o retro_snake retro_snake.cpp -lraylib -lm -lpthread -ldl -lrt -lX11
    ```
4. Run the executable:
    ```bash
    ./retro_snake
    ```

## License:
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

# Tetris Clone

This project is a simple Tetris clone written in C using the Raylib library for graphics rendering. It's designed to run on desktop platforms.

## Features

- Basic Tetris gameplay mechanics
- Collision detection
- Score tracking
- Next and saved piece preview
- Game over detection and restart

## Prerequisites

To compile and run this project, you need to have the following installed:

- GCC compiler
- Raylib library

## Compilation

The project includes a Makefile for easy compilation. Use the following commands:

```sh
make main
```

This will compile the source code into an executable named [`main`].

## Running the Game

After compilation, you can start the game using:

```sh
make run
```

Or directly:

```sh
./main
```

## Controls

- Arrow keys for moving the pieces.
- Space to save a piece for later.
- Enter to restart the game after a game over.

## Configuration

The game's settings can be adjusted in the [`main.c`] file, including screen size, block size, and colors.

## License

This project is open-source and available under the MIT License.

## Acknowledgments

- Raylib library for providing a simple and easy-to-use platform for game development.

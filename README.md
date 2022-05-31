# Conway's Game of Life

An implementation of the [Conway's Game of Life cellular automata](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) in C++.

## Rules

There are 4 rules in Conway's Game of Life:

1. If a living cell has less than 2 neighbours, it dies of underpopulation.
2. Any living cell with either 2 or 3 neighbours lives on to the next generation.
3. If a living cell has more than 3 neighbours, it dies of overpopulation.
4. If a dead cell has exactly 3 neighbours, it becomes alive, due to repopulation.

## Running Conway's Game of Life

### Compiling

To compile this project, first download the source code from the repocitory with it's submodules.

```console
foo@bar:~$ git clone https://github.com/jovialen/conways_game_of_life.git --recursive
```

Then compile the project with `cmake`.

After compiling, you can run the application with the executable file named `cgol`.

### Keybindings

| Key         | Description           |
| ----------- | --------------------- |
| SPACE       | Pause the simulation  |
| ENTER       | Resume the simulation |
| LEFT MOUSE  | Place living cell     |
| RIGHT MOUSE | Kill cell             |

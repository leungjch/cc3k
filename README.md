# CC3K - CS 246 Final Project

[![C/C++ CI](https://github.com/leungjch/cc3k/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/leungjch/cc3k/actions/workflows/c-cpp.yml)

## Setup
Graphics requires `libxpm-dev` (for drawing pixelmaps on Xwindow) and `libx11-dev` (for Xwindow). `libx11-dev` should be installed by default on Linux, but you may need to install `libxpm-dev`:

```sh
sudo apt install libxpm-dev
```

`libxpm-dev` was checked to be installed on all of the Linux student environments (verified by an instructor) so it does not need to be installed there. 

Run `make` to compile the project, then run `./cc3k` to launch the game.

## Command line Arguments
### Read in a custom layout
Run with a filename passed in to load a custom layout for each of the five floors. **Note:** Exactly five floors are expected. See `customMap.txt` for an example of a valid file. 

E.g.
```sh
./cc3k customMap.txt
```

### DLC (extra credit features)
Run `cc3k` with `dlc` to enable extra features.
```sh
./cc3k dlc
```

## Controls
-  **no,so,ea,we,ne,nw,se,sw**: moves the player character one block in the appropriate cardinal direction.
-  **u direction**: uses the potion indicated by the direction (e.g. no, so, ea).
-  **a direction**: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must be one block north of the @).
-  **s, d, v, g, t**: specifies the race the player wishes to be when starting a game.
-  **f**: stops enemies from moving until this key is pressed again.
-  **r**: restarts the game. All stats, inventory, and gold are reset. A new race should be selected.
-  **q**: allows the player to admit defeat and exit the game.

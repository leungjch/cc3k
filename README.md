# CC3K - CS 246 Final Project

[![C/C++ CI](https://github.com/leungjch/cc3k/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/leungjch/cc3k/actions/workflows/c-cpp.yml)

## Compilation
Graphics requires `libxpm-dev` (for drawing pixelmaps on Xwindow) and `libx11-dev` (for Xwindow). `libx11-dev` should be installed by default on Linux, but you may need to install `libxpm-dev`:

```sh
sudo apt install libxpm-dev
```

`libxpm-dev` was checked to be installed on all of the Linux student environments (verified by an instructor) so it does not need to be installed.

Run `make` to compile the project, then run `./cc3k` to launch the game.

# Raylib Breakout Game

A simple Raylib game example that uses git submodule to fetch raylib and uses cmake to build desktop, and web target

## Prerequisites

- emscripten installed if you wanted to target web
- shell ofcourse
- make and gcc

## Usage

```sh
git clone https://github.com/UnknownRori/raylib-breakout-game

cd raylib-breakout-game

# So it won't take so long :)
git submodule update --init --depth 1

# Generate and run Desktop build
cmake -S . -B build
cmake --build ./build/

# Generate and run Web build
# I know you guys can use emscripten, there is the docs to install it
emcmake cmake -S . -B web-build -DPLATFORM=Web
cmake --build ./web-build
```

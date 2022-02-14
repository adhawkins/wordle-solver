# Wordle Solver

## Introduction

This project provides a console application, web page and Chrome extension for solving [Wordle](https://www.nytimes.com/games/wordle/index.html) puzzles.

It also works with [Wordle2](https://www.wordle2.in/).

## Requirements

The project is built using [cmake](https://cmake.org/).

- The console application requires no external dependencies.
- The web-based application and Chrome Extension require [emscripten](https://emscripten.org/) to compile the C++ library code into a Web Assembly.

## Console Application

### Building the Project

1. Create a directory to build in (`mkdir console-build`)
2. Change into this directory (`cd console-build`)
3. Invoke cmake to create build environment (`cmake ..`)
4. Build the software (`make`)
5. Create a package containing the software (`make package`)

In the build directory, a ZIP file will be created (named like `wordle-solver-1.0.0-Linux-console.zip`) containing the console application and sample word lists.

### Usage

Extract the ZIP file created during the build step.

`Usage: ./wordle-solver wordlistfile wordlength [initial guess word]`

**Sample invocation**

`bin/wordle-solver word-lists/wordle-words.txt 5`

The software will provide you with a 'guess' to enter. This guess should be entered into Wordle, and the marks entered into the software. One digit should be entered for each letter as follows:

- 0 - The letter does not appear in the word (dark grey)
- 1 - The letter appears in the word, but is in the wrong location (yellow)
- 2 - The letter appears in the word, and is in the correct location (green)

If Wordle indicates that the guess is invalid, simply enter 'z' and another guess will be provided.

Further guesses should be entered into Wordle and marked until the correct word is provided. Mark the word using the digit '2' in all places to indicate this.

## Webpage Application

### Building the project

1. Install and activate emscripten.
2. Create a directory to build in (`mkdir web-build`)
3. Change into this directory (`cd web-build`)
4. Invoke cmake to create build environment (`emcmake cmake ..`)
5. Build the software (`make`)
6. Create a package containing the software (`make package`)

In the build directory, a ZIP file will be created (named like `wordle-solver-1.0.0-Emscripten-web.zip`) containing the web application files.

### Usage

Extract the ZIP file created during the build step to a directory on a web server. Visit this URL.

A web page similar to the Wordle site will be displayed. Each guess will be indicated in a row. This should be entered into Wordle, and then marked to match that shown on the Wordle site. Click on each letter to turn it the correct colour to match those on Wordle. Once all letters have been marked, click 'Mark' and the next guess will be provided.

If the guess given is rejected by Wordle, click 'Invalid Word' and a new guess will be given.

Further guesses should be entered into Wordle and marked until the correct word is provided.

## Chrome Browser Extension

### Building the project

Build the project as in [Webpage Application](#webpage-application).


In the build directory, a ZIP file will be created (named like `wordle-solver-1.0.0-Emscripten-chrome-extension.zip`) containing the Chrome Extension files.

### Usage

The extension needs to be installed in Chrome using 'developer mode':

1. Extract the ZIP file to a directory on the machine running Chrome.
2. In Chrome, select `Menu -> More tools -> Extensions`.
3. At the top right, turn on 'Developer mode'.
4. Click on 'Load unpacked'.
5. Browser to the directory containing the extracted extension files.

The extension should now be installed. You may want to click on 'Details' for the extension, and enable 'Allow in Incognito' in order to be able to use the extension in an Incognito window (after completing each daily Wordle puzzle as normal).

To use the extension, simply open the Wordle (or Wordle2) site. The extension's button should become enabled. Clicking on the extension's button should automatically enter guesses into the puzzle until it is solved, or all guesses are used up.

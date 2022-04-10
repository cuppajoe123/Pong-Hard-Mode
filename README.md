# Pong: Hard Mode
This project is inspired by Pong, implemented in C using the SDL graphics library. 

Similar to Pong, but a single player controls both paddles and there are two balls instead of one. The player gains a point each time they hit a ball and loses when a ball gets past a paddle.

The user can also enter a username, which, along with their final score, will be written to a file to be read from when the leaderboard is rendered after a game..

Uses the Fira Code font.

### Dependencies and installation
These are the packages that need to be installed on Debian-based systems. Other distros will have similar packages.

`# apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

Invoke the Makefile to build the program in the current directory.

`$ make`

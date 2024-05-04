``Reupload project from 2021 : This project was recovered so don't take commits into accounts``

<h1 align="center"> Inquisition :video_game: :space_invader: <h1>

<img src="https://img.shields.io/badge/platform-gameboy-yellowgreen" />
<img src="https://img.shields.io/badge/language-c-lightgrey" />


## Preview
![Preview](https://i.ibb.co/W6133Gm/Capture-d-e-cran-2024-01-23-a-22-51-54.png)


## Overview
This project is a Gameboy game where you play a Judge that need to defend his village attacked by some bandits,
bandits come by waves 1, 2 or 3 at time and you have to smash your hammer to kill them. You can also send a shockwave to them but careful only once at time !


The github file already provie the ``.gb``, so just drag n drop it into a Gameboy Emulator [Bgb for windows user](http://bgb.bircd.org/) or [mGBA for linux and mac user](https://mgba.io/)


The game was made in C using the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020) for Nintendo Gameboy and more


## Controls
Beware to set your emulator control befeore !

A -> Smash Hammer/Skip Score Screen

B -> Send Shockwave

Start -> Start the game


## Build the project

If you want to build the project you can follow the step below :
- Ensure you have GCC installed
- Download the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020)
- Put the GBDK folder where you want but copy the path to it
- Then open the Makefile and set the ``GBDK_FOLDER`` var at the path you got (keep the / at the end of the GBDK path)
- Finally just run ``make`` command this will replace the .gb by a freshly builded one
- Then you can just drag n drop it into a GB emulator
- (Optionnal) if you got lot of compilation file you can run ``make clean``

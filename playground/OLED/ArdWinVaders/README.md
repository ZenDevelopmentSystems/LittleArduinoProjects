# OLED/ArdWinVaders

An Arduino & OLED version of a classic game.

Here's a quick video of the game in action:

[![ArdWinVaders](http://img.youtube.com/vi/m0XgdGztr8s/0.jpg)](http://www.youtube.com/watch?v=m0XgdGztr8s)

## Notes

Since playing around with driving an OLED with an Arduino (see the [SSD1306BareBack](../SSD1306BareBack) project for example),
it doesn't take long for the mind to wander into gaming territory...

So here's the first version of a retro classic: Space Invaders in full 128x64 mono resolution running on an Arduino Pro Mini at 8MHz!

For details and discussion of the hardware setup, see the [SSD1306BareBack](../SSD1306BareBack) project.
This build is mainly about the software.

The software is organised into a number of classes. You can see the code for more details, but here is the overview:
* ArdWinVaders.ino - main program, also handles all the controls input (fire, movement)
* game_board.h/cpp - implements the main game controller class, responsible for orchestrating actions and sequencing gameplay
* buffered_display.h/cpp - implements a logical display interface that GameBoard talks to. It maintains the screen buffer and handles movement, collision detection and so on.
* oled_driver.h/cpp - implements the OLED-specific display driver. It goes direct to the hardware via SPI.


### TODO

* make it faster: gameplay is slowed especially by the overhead of SPI to do a full screen refresh. A few optimisations in mind:
  - implement some fast SPI (similar to the what the Adafruit_SSD1306 library does)
  - implement smart/selective screen refresh - currently it does naive full screen refresh
  - run the processor at > 8MHz
* make it noisy: yes, some music and sound effects would be nice, even on a piezo!
* put it in a hand-held form-factor


## Construction

![The Breadboard](./assets/ArdWinVaders_bb.jpg?raw=true)

![The Schematic](./assets/ArdWinVaders_schematic.jpg?raw=true)

![The Build](./assets/ArdWinVaders_build.jpg?raw=true)

## Credits and References
* [SSD1306 datasheet](https://www.adafruit.com/datasheets/SSD1306.pdf)
* [SSD1306BareBack](../SSD1306BareBack) - a sketch the exlores and explains OLED driving basics
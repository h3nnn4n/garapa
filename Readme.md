Here comes dat gameboi
======================
LELmark
-------

LELmark is a fork (right now a branch) of Here comes dat gameboi that plays Tetris by itself. So far the plan is to make a couple of Game specific AI to play some games.

Features
========

 - Reading the placed blocks from OAM
 - Reading the falling piece from OAM
 - Identifying the falling piece directly from the game memory
 - Identifying the current game screen directly from the game memory
 - 8 different metrics to evaluate where the piece will go
 - Genetic algorithm to weight the different placement criteria
 - Fitness based on the number of cleared lines
 - Autonomous training system
 - For each individual, the game is played 5 times and the worst run is used as fitness, to ensure consistency.

License
=======
See `LICENSE` for more info.

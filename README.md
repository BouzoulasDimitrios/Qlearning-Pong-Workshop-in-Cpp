# Implementation of Qleaning for Pong using C++ & SFML

<p align = "center">
<img src="./images/pongplayer.jpeg" alt="Anime pong player image." style="width:50%;"/>
</p>



# Introduction

This is an implementation of a qlearning algorithm for the classic game of pong.<br> 
In this implementation standard c++ libraries are used in order to implement qlearning.<br> 
No external machine learning libraries are used as the implementation does not involve a neural network.<br> 

# Setup

Prerequisites: [SFML](https://www.sfml-dev.org/index.php)

    $ git clone https://github.com/BouzoulasDimitrios/Qlearning-Pong.git
    $ cd Qlearning-Pong/
    $ bash launch.sh

# Demo

**Model on the Left vs me on the Right.**

<p align = "center">
<img src="./images/Demo.gif" alt="Demo gif" style="width:50%;"/>
</p>

# Detailed explanaition

This model was created using a reinforcement learning algorithm called [Qlearning](https://en.wikipedia.org/wiki/Q-learning). <br>

Qleaning uses game states in order to describe what is happening in the game. <br>

For example the starting game state: <br>

<p align = "center">
<img src="./images/starting_gamestate.png" alt="exaple game state." style="width:50%;"/>
</p>

In any given state of the PONG game we can describe what is happening by simply listing the game components coordintes on the game window.<br>

In the start of the game we can describe the game as following:

Right paddle: x = 1000 , y = 228
Left  paddle: x = 10   , y = 228
Ball: x = 507, y = 251  

As a result the whole game can be described at any state by six numbers.

### How Qlearning works

In simple terms qlearning aims to create a table which will give us the correct move for every game state.

In pong we have 3 moves only for our model. UP, DOWN or STAY.

As a result we get a lookup table that looks like this:

|          |    UP   |  STAY   |  DOWN   |   
|----------|---------|---------|---------|
|  state 0 | <span style="color:green">correct</span>  | <span style="color:red">wrong</span>     | <span style="color:red">wrong</span> |
|  state 1 | <span style="color:red">wrong</span>      | <span style="color:green">correct</span> | <span style="color:red">wrong</span> |
|  state 2 | <span style="color:green">correct</span>  | <span style="color:red"> wrong </span>   | <span style="color:red">wrong</span> |




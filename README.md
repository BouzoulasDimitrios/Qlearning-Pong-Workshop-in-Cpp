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

## summary


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

As a result the whole game can be described at any state by six numbers.<br>
<br>


### How decisions are made

In simple terms qlearning aims to create a table which will give us the correct move for every game state.

In pong we have 3 moves only for our model. UP, DOWN or STAY.

As a result we get a lookup table that looks like this:


|          |    UP   |  STAY   |  DOWN   |   
|----------|---------|---------|---------|
|  state 0 | ![#c5f015](https://placehold.co/15x15/c5f015/c5f015.png) **correct**  | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong**     | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong** |
|  state 1 | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong** | ![#c5f015](https://placehold.co/15x15/c5f015/c5f015.png) **correct**   | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong** |
|  state 2 | ![#c5f015](https://placehold.co/15x15/c5f015/c5f015.png) **correct**  | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong**     | ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) **wrong** |


But how does the algorithm know which action it should take for any given game state?<br>
This is where **training** takes place, the algorithm takes random actions and gets rewarded or punished based on the action it takes.<br>
After training for a while the lookup table will end up having large numeric values for the correct moves and negative ones for the wrong moves.


|          |    UP   |  STAY   |  DOWN   |   
|----------|---------|---------|---------|
|  state 0 |  0.929 | -0.892 | -0.932 |
|  state 1 | -0.988 |  0.922 | -0.894 |
|  state 2 |  0.981 | -0.878 | -0.825 |


Now during the testing phase the only thing that our model has to do is to look at the highest number for a given game state and take that action. <br>
If it was trained correctly it should be able to play the game.

# Qlearning
## Goal

The goal we have is a model that can play **pong**. <br>
Due to the simplicity of the approach it can be difficult to make the model perform fancy moves<br>
and teach it to hit the ball with nice angles.

As a result the problem is approached with a simpler goal.<br>
**"Make the model always hit the ball"**.<br>
That will be the goal for the model, and the reason for that is simple.<br>
In pong if you always hit the ball you cannot lose as a result you will win sooner or later.<br>
<br>

## calculating game states

This is one of the first problems that come up.<br>
In order to calculate all the possible game states we run into a problem.<br>
There are simply too many of them.<br>

In case we want to calculate all of the game states we have the following variables:<br>


    Right paddle: x, y
    Left  paddle: x, y
    Ball: x, y   


Now some of these variables are useless as the paddles do not move in the **X** axis.<br>

As a result we are left with the following game variables:<br>

    Right paddle: y
    Left  paddle: y
    Ball: x, y 

**Y values** range from 0 to 512 <br>
**X values** range from 0 to 1024 <br>
<br>

    Calculating the game states gives us the following number:

    game_states = 512 * 512 * 512 * 1024 => 
    game_states = 137438953472 

This leaves us with roughly 100 Billion game states which is an unpleasantly big number.<br>

As a result we should try to get it smaller.<br>
Let's start by the least relevant part, the oponents paddle height.<br>
If my goal is to hit the ball I do not have to know where the oponents paddle is.<br>

    So let's calculate the ammount of game states again leaving out the oponents paddle Y position:

    game_states = 512 * 512 * 1024 => 
    game_states = 268435456 

Still being in the hundreds of millions is not that optimal as we need to have 3 values for every game state<br>
leaving us with nearly 1 billion values.<br>

So how to optimize from here? <br>

we could apply the following approach:<br>
As we aim to always hit the ball and nothing else, the only thing that we need to do is to be on the same level as the ball.<br>

We could simply use the **ball's Y** value and the **paddle's Y** value.

    Now let's calculate again the game states without the ball's X:

    game_states = 512 * 512 => 
    game_states = 262144 

This leaves us with a number which is in the hundreds of thousands instead of hundreds of billions which is a significan improvement.

Now what the model will see is just two heights like this:


<p align = "center">
    <img src="./images/PaddleBallHeights.png" alt="Bar plot of paddle and ball." style="width:70%;"/>
</p>

The result is a model that will try to always match these heights, simmilar to the middle `Same height` plot, if the actions of the model result in the paddle being bellow or above the ball, it gets punished.

Now that we have the values that will make up our states, we need to create some function in order to calculate the state.


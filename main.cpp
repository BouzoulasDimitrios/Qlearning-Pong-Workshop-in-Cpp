/**
 * launch the game: g++ main.cpp -o myapp -lsfml-graphics -lsfml-window -lsfml-system && ./myapp 
*/

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>

#include <vector>
#include <fstream>
#include <math.h>

#include <chrono>
#include <thread>
#include <string>

#include "./resources/pong.h"
#include "./resources/qlearning.h"
#include "./resources/utilities.h"

using namespace std;

const int QPADDLE_MOVE = 15;

const int NUMBER_OF_ACTIONS = 3;
const int NUMBER_OF_STATES  = 262144*2;//10000;//3000;

float learning_rate = 0.7;
float discount_rate = 0.99;

float exploration_rate = 1;
float max_exploration_rate = 1; 
float min_exploration_rate = 0.01;
float exploration_decay_rate = 0.01;

long long int game_loop = 0;

int game_state_start;
int game_state_end;
int action;
float reward = 0;
int old_save_val = 0;

std::size_t range = 262144;
std::string filename = "./resources/weights.txt";
std::vector<std::vector<float>> Qtable = std::vector<std::vector<float>>(2*range, std::vector<float>(NUMBER_OF_ACTIONS, 0.0f));

State start_state;
State end_state;

std::ostream &operator<<(std::ostream & stream, const State & state_of_game){ return stream << "ball x =  " <<state_of_game.ball_x << " ball y =  " <<state_of_game.ball_y << " paddle 1 =  "<< state_of_game.paddle1<< " paddle 2 =  "<< state_of_game.paddle2<<endl; }

const int tableSize = 512;
std::vector<std::vector<int>> lookupTable(tableSize, std::vector<int>(tableSize, 0));

void init_state_table(){

    int value = 0;
    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            lookupTable[i][j] = value;
            value++;
        }
    }

    // Print the lookup table
    for (const auto& row : lookupTable) {
        for (int val : row) {
            // std::cout << val << " ";
        }
        // std::cout << std::endl;
    }

    return;
}

int state_calc(State & state, float velocity){

    std::cout<<"ball pad "<<state.ball_y<< "   " <<state.paddle1<<endl;
    std::cout<<"State:  "<<lookupTable[state.ball_y][state.paddle1]<< "     "<<velocity<<endl;

    if(velocity > 0){
        cout<<"POSTITIVE"<<velocity<<endl; //state.ball_x, 
        return range + lookupTable[state.ball_y][state.paddle1];     //5000 + state.ball_x + 2*state.ball_y + state.paddle1;
    }else{
        cout<<"NEGATIVE"<<velocity<<endl; //state.ball_x, 
        return range - lookupTable[state.ball_y][state.paddle1];     //5000 - state.ball_x + 2*state.ball_y + state.paddle1;
    }

    return lookupTable[state.ball_y][state.paddle1];

}

int correct_move_calculator(State state1, float vel){

    cout<< vel<<endl;
    if(vel >= 0){
        cout<<"going down"<<endl;

        if(state1.paddle1 + 28 > state1.ball_y){
            cout<<"paddle below ball 1 "<< state1.paddle1 + 28 << " pad , ball " << state1.ball_y <<endl;
            return 1;
        }else if(state1.paddle1 + 28 < state1.ball_y){
            cout<<" 2 "<< state1.paddle1 + 28 << " pad , ball " << state1.ball_y <<endl;
            return 2;
        }

    }else{
        cout<<"going up"<<endl;
        if(state1.paddle1 + 28 > state1.ball_y){
            cout<<" paddle bellow ball 0 "<< state1.paddle1 + 28 << " pad , ball " << state1.ball_y <<endl;
            return 0;
        }else if(state1.paddle1 + 28 < state1.ball_y){
            cout<<" 1 paddle above ball 1 "<< state1.paddle1 + 28 << " pad , ball " << state1.ball_y <<endl;
            return 1;
        }
    }


    return 0;
}


int main()
{
    init_state_table();
    // select game mode
    int game_mode = game_mode_selection();

    // cout<<"filename? "<<endl;
    // cin>>filename;
    load_table(filename, Qtable);
    PongGame pg;

    //needed for random values;
    srand(time(0));
    pg.game_window.setKeyRepeatEnabled(false);
    wait_to_start(pg);

    // run the program as long as the window is open
    while (pg.game_window.isOpen())
    {
        cout<<" start of loop "<<endl;

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (pg.game_window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) pg.game_window.close();
            if (event.type == sf::Event::KeyPressed) handle_keypress(pg, event, game_mode);
        }

        pos = pg.ball.getPosition();
        l_paddle_pos = pg.left_paddle.getPosition();
        r_paddle_pos = pg.right_paddle.getPosition();
        pos.x += velX;
        pos.y += velY;

        //starting state
        start_state.ball_x = pos.x;
        start_state.ball_y = abs(pos.y);
        start_state.paddle1 = l_paddle_pos.y;

        //calculate starting game state
        game_state_start = state_calc(start_state, velY);//start_state.ball_x + (2*start_state.ball_y) + start_state.paddle1;

        int correct_move = correct_move_calculator(start_state, velY);

        // Action selection & implementation
        if(game_state_start < range*2)
        action = select_action(game_state_start, Qtable, exploration_rate);

        // move paddle based on selected action
        if(game_mode >= 2)take_action(pg, action, QPADDLE_MOVE);

        // POINT SCORING HANDLING
        if((pos.x > pg.game_window.getSize().x - pg.ball.getRadius() * 2.f) || (pos.x < 0.f)){ point_scrored(pg, game_mode, reward, pos.x); }

        // Ball bouncing off of the top & bottom of the game
        if (pos.y < 0.f || pos.y > pg.game_window.getSize().y - pg.ball.getRadius() * 2.f) { velY = -velY; } 

        left_paddleBounds = pg.left_paddle.getGlobalBounds();
        right_paddleBounds = pg.right_paddle.getGlobalBounds();
        ballBounds = pg.ball.getGlobalBounds();

        if ( left_paddleBounds.intersects(ballBounds) ) {
            float t = ((pos.y - l_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = fabs(velX) + 0.1;
            velY = t*2.8;

        }else if( right_paddleBounds.intersects(ballBounds) ){
            float t = ((pos.y - r_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = -fabs(velX) - 0.1;
            velY = t*2.8;
        }
        
        pg.ball.setPosition(pos);

        // automated player -> fuctions
        if(game_mode == 3)
            perfect_player(pg);
        
        /// paddle = 56 /2 = 28, ball = 10/2 = 5 
        int starting_distance = start_state.paddle1 - start_state.ball_y + 28 ;
        int end_distacne      = end_state.paddle1   - end_state.ball_y   + 28 ;        
// rand()%2 ? -1.0f : 1.0;
        reward = (action == correct_move) ? 1 : -1;

        cout<<" reward 3 = "<<reward<<endl;

        //end state;
        pos = pg.ball.getPosition();
        l_paddle_pos = pg.left_paddle.getPosition();
        r_paddle_pos = pg.right_paddle.getPosition();
        end_state.ball_x = pos.x;
        end_state.ball_y = abs(pos.y);
        end_state.paddle1 = l_paddle_pos.y;

        // game_state_end = end_state.ball_x + 2*end_state.ball_y + end_state.paddle1;
        cout<<"end distance ; "<<end_distacne<<endl;
        game_state_end = state_calc(end_state, velY);// (end_state.ball_x*SCREEN_WIDTH + abs(end_state.ball_y* SCREEN_HEIGHT) + SCREEN_HEIGHT * end_state.paddle1)/2;


        update_qtable(Qtable, game_state_end, game_state_start, learning_rate, discount_rate, reward, action);

        // Update exploration rate
        exploration_rate = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * exp(-exploration_decay_rate * (r_points + 1000));

        pg.draw();
        pg.game_window.display();  

        cout<< " model state: \nlearning rate = "<<learning_rate<<"exploration_Rate = "<<exploration_rate<<" reward = "<<reward<<endl;

        int total_points = r_points + l_points ;
        if(total_points % 10 == 0 && total_points != old_save_val)
          save_table(filename, Qtable), old_save_val = total_points;
        

        //reset reward
        reward = 0;

        // Delay used for debugging
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        system("clear");

    }

    return 0;
}

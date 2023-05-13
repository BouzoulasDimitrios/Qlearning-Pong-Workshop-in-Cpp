#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//$ g++ clean.cpp -o myapp -lsfml-graphics -lsfml-window -lsfml-system && ./myapp
#include <cmath>
#include <string>
#include <iostream>

#include <vector>
#include <fstream>
#include <math.h>

#include <chrono>
#include <thread>
#include <string>

#include "../resources/pong.h"
#include "../resources/qlearning.h"

using namespace std;

const int QPADDLE_MOVE = 15;

const int NUMBER_OF_ACTIONS = 3;
const int NUMBER_OF_STATES  = 1000000;//3000;

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
float reward;
int old_save_val = 0;

char * test;
std::string filename;// = "teat01.txt";

std::vector<std::vector<float>> Qtable = std::vector<std::vector<float>>(NUMBER_OF_STATES, std::vector<float>(NUMBER_OF_ACTIONS, 0.0f));

struct State {
  float paddle1, paddle2; // paddle positions
  float ball_x, ball_y;   // ball position
};

State start_state;
State end_state;

std::ostream &operator<<(std::ostream & stream, const State & state_of_game){
  return stream << "ball x =  " <<state_of_game.ball_x << " ball y =  " <<state_of_game.ball_y << " paddle 1 =  "<< state_of_game.paddle1<< " paddle 2 =  "<< state_of_game.paddle2<<endl;
}


void update_qtable(){
    int next_move = static_cast<int> (*max_element(std::begin(Qtable[game_state_end]), std::end(Qtable[game_state_end])));
    Qtable[game_state_start][action] = Qtable[game_state_start][action] * (1 - learning_rate) + learning_rate * (reward + discount_rate * next_move);
}

bool check_number(string str) {
   for (int i = 0; i < str.length(); i++)
   if (isdigit(str[i]) == false)
      return false;
      return true;
}

int game_mode_selection(){

    string user_input;
    int mode = -1;
    cout << "Enter game mode:\n1) Player vs Player \n2)Player vs AI 3) AI vs Hardcoded player"<<endl; // Type a number and press enter
    cin >>user_input;
    if(check_number(user_input))
        mode = stoi(user_input);

    while(mode<1 || mode>4){
        cout << "Wrong entry. \n"; // Type a number and press enter
        cout << "Enter game mode:\n1) Player vs Player \n2)Player vs AI 3) AI vs Hardcoded player"<<endl; // Type a number and press enter
        cin >>user_input;
        if(check_number(user_input))
            mode = stoi(user_input);
    } 

    if(mode == 1)
        cout<<"Left Player controls: 's' UP  'w' DOWN \n Right Player controls: 'UP arrow' UP  'DOWN arrow' DOWN "<<endl;
    else if(mode == 2)
        cout<<"You control the right paddle, Player controls: 'UP arrow' = UP  'DOWN arrow' = DOWN "<<endl;
    else if(mode == 3)
        cout<<"Watch the Qlearning algorithm play against a perfect hardcoded player";

    cout<< "Please, press any Key to start the game \n";

    return mode;

}



int main()
{
    int game_mode = game_mode_selection();
    // game_mode_selection();

    cout<<"filename? "<<endl;
    cin>>filename;

    // load_table("last01.txt", Qtable);
    // load_table(filename, Qtable);
    // print_table(Qtable);

    //needed for random values;
    srand(time(0));

    PongGame pg;
    pg.game_window.setKeyRepeatEnabled(false);

    sf::Vector2f pos;
    sf::Vector2f l_paddle_pos;
    sf::Vector2f r_paddle_pos; 
    sf::Vector2f ball_pos;

    wait_to_start(pg);

    // run the program as long as the window is open
    while (pg.game_window.isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (pg.game_window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                pg.game_window.close();

            if (event.type == sf::Event::KeyPressed)
            {    
                if((event.key.code == sf::Keyboard::W && l_paddle_pos.y - PADDLE_MOVE > 0) && game_mode == 1){
                    pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y - PADDLE_MOVE);
                }
                else if((event.key.code == sf::Keyboard::S && l_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE) && game_mode == 1){
                    pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y + PADDLE_MOVE);
                }
                else if((event.key.code == sf::Keyboard::Up && r_paddle_pos.y - PADDLE_MOVE > 0 ) && game_mode <= 2){
                    pg.right_paddle.setPosition(r_paddle_pos.x, r_paddle_pos.y - PADDLE_MOVE);
                }
                else if((event.key.code == sf::Keyboard::Down && r_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE) && game_mode <= 2){
                    pg.right_paddle.setPosition(r_paddle_pos.x, r_paddle_pos.y + PADDLE_MOVE);
                }
            }

        }

        pos = pg.ball.getPosition();
        l_paddle_pos = pg.left_paddle.getPosition();
        r_paddle_pos = pg.right_paddle.getPosition();
        
        pos.x += velX;
        pos.y += velY;

        //starting state

        start_state.ball_x = pos.x;
        start_state.ball_y = pos.y;
        start_state.paddle1 = l_paddle_pos.y;

        // game_state_start = start_state.ball_x + (2*start_state.ball_y) + start_state.paddle1;
        game_state_start =  (SCREEN_HEIGHT * start_state.ball_x + abs(SCREEN_HEIGHT * start_state.ball_y) +  SCREEN_HEIGHT * start_state.paddle1)/2;
        cout<<start_state<<endl;

        // Action selection & implementation
        action = select_action(game_state_start, Qtable, exploration_rate);

        if(game_mode >= 2){
            cout<<"TEST TEST";
            if( action == 0 && l_paddle_pos.y - QPADDLE_MOVE > 0 )
                pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y - QPADDLE_MOVE);
            else if( action == 2 && l_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - QPADDLE_MOVE)
                pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y + QPADDLE_MOVE);
        }

        // POINT SCORING
        if (pos.x < 0.f)
        {
            r_points += 1;       
            
            if(r_points == 10 && game_mode <= 2){                
                l_points = 0, r_points = 0;
                pg.end_game(false);
            }

            velX = -velX;
            pg.left_score.setString(to_string(r_points));
            cout<<"+ 1 to the right player "<<r_points<<endl;
            pg.reset();
            pos = pg.ball.getPosition();        
            initial_ball_speed();
            reward = -1;

        }else if(pos.x > pg.game_window.getSize().x - pg.ball.getRadius() * 2.f){

            l_points += 1;
            if(l_points == 10 && game_mode <= 2){
                pg.end_game(true);
                l_points = 0, r_points = 0;
            }
                
            velX = -velX;
            cout<<"+ 1 to the left player "<<l_points<<endl;
            pg.right_score.setString(to_string(l_points));
            pg.reset();
            pos = pg.ball.getPosition();
            initial_ball_speed();
            reward = 1;
        }   

        // Ball bouncing off of the top & bottom of the game
        if (pos.y < 0.f || pos.y > pg.game_window.getSize().y - pg.ball.getRadius() * 2.f) { velY = -velY; } 
        

        sf::FloatRect left_paddleBounds = pg.left_paddle.getGlobalBounds();
        sf::FloatRect right_paddleBounds = pg.right_paddle.getGlobalBounds();
        sf::FloatRect ballBounds = pg.ball.getGlobalBounds();

        if ( left_paddleBounds.intersects(ballBounds) ) {
            cout<<"left colision"<<endl;            
            float t = ((pos.y - l_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = fabs(velX) + 0.02;
            velY = t*0.5;
            reward = 1;

        }else if( right_paddleBounds.intersects(ballBounds) ){
            cout<<"left colision"<<endl;
            float t = ((pos.y - r_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = -fabs(velX) - 0.02;
            velY = t*0.5;
        }
        

        pg.ball.setPosition(pos);

        float x , y;


        // automated player -> fuctions
        if(game_mode == 3)
        {
            if(r_paddle_pos.y > pos.y){
                x = r_paddle_pos.x, y =  r_paddle_pos.y - 5;
            }else if(r_paddle_pos.y < pos.y - 5){
                x = r_paddle_pos.x, y = pos.y + 5;
            }
    
            pg.right_paddle.setPosition(x , y);

        }


        //reward calculation -> function
        int starting_distance = start_state.paddle1 -14 - start_state.ball_y;
        int end_distacne      = end_state.paddle1   -14 - end_state.ball_y;
        
        if(end_distacne <= 28 && end_distacne >= -28){
            reward += 0.5;
        }else if( abs(starting_distance) > abs(end_distacne)){
            reward += 0.2;
        }

        //end state;
        pos = pg.ball.getPosition();
        l_paddle_pos = pg.left_paddle.getPosition();
        r_paddle_pos = pg.right_paddle.getPosition();
        end_state.ball_x = pos.x;
        end_state.ball_y = pos.y;
        end_state.paddle1 = l_paddle_pos.y;

        // game_state_end = end_state.ball_x + 2*end_state.ball_y + end_state.paddle1;
        game_state_end = (end_state.ball_x*SCREEN_WIDTH + abs(end_state.ball_y* SCREEN_HEIGHT) + SCREEN_HEIGHT * end_state.paddle1)/2;
        cout<<game_state_start<<endl;

        cout<<"\n\n game state = "<<end_state<<endl;
        
        update_qtable();

        // Update exploration rate
        exploration_rate = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * exp(-exploration_decay_rate * (r_points));

        pg.draw();
        pg.game_window.display();  

        cout<< " model state: \nlearning rate = "<<learning_rate<<"\nexploration_Rate = "<<exploration_rate<<"\n reward = "<<reward<<endl;

        if(r_points % 10 == 0 && r_points != old_save_val){
          save_table(filename, Qtable);
          old_save_val = r_points;
        }

        //reset reward
        reward = 0;
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }

    return 0;
}

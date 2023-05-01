
#ifndef PONG

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const int MULTIPLIER = 2;

const int SCREEN_WIDTH  = 512*MULTIPLIER;
const int SCREEN_HEIGHT = 256*MULTIPLIER;

const int PADDLE_HEIGHT = 28 * MULTIPLIER;
const int PADDLE_WIDTH  = 7 * MULTIPLIER;

const int L_PADDLE_X_POS = 10;
const int R_PADDLE_X_POS = SCREEN_WIDTH - 10 - PADDLE_WIDTH;

const int BALL_RADIUS = 10;
const int PADDLE_MOVE = 30;

int ball_x = SCREEN_WIDTH/2 - BALL_RADIUS/2, ball_y = SCREEN_HEIGHT/2 - BALL_RADIUS/2;

int l_paddle_y_pos = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
int r_paddle_y_pos = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2; 

int x_speed = 10;
int y_speed = 10;

float velX = 0.f;
float velY = 0.f;

const float MAX_BOUNCE_ANGLE = 60.0f; // Maximum angle of bounce in degrees

int l_points = 0, r_points = 0;

class PongGame{

    public:

    sf::RenderWindow game_window =  sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "My window");

    sf::RectangleShape left_paddle = sf::RectangleShape(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    sf::RectangleShape right_paddle = sf::RectangleShape(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    sf::CircleShape ball = sf::CircleShape(10);

    sf::Text left_score;
    sf::Text right_score;
    sf::Font font;

    PongGame(){
        ball.setPosition(ball_x, ball_y);
        left_paddle.setPosition(L_PADDLE_X_POS, l_paddle_y_pos);
        right_paddle.setPosition(R_PADDLE_X_POS, r_paddle_y_pos);
        font.loadFromFile("../resources/arial.ttf");

        left_score.setFont(font);
        right_score.setFont(font);  

        left_score.setString(std::to_string(l_points));
        left_score.setCharacterSize(24);
        left_score.setFillColor(sf::Color::White);
        left_score.setPosition(412 - 5, 50);

        right_score.setString(std::to_string(r_points));
        right_score.setCharacterSize(24);
        right_score.setFillColor(sf::Color::White);
        right_score.setPosition(612 - 5, 50);
    }

    void draw(){
        game_window.clear();
        game_window.draw(left_paddle);
        game_window.draw(right_paddle);
        game_window.draw(ball);
        game_window.draw(left_score);
        game_window.draw(right_score);
        game_window.display();  
    }

    void reset(){
        
        ball_x = SCREEN_WIDTH/2 - BALL_RADIUS/2, ball_y = SCREEN_HEIGHT/2 - BALL_RADIUS/2;

        ball.setPosition(ball_x, ball_y);
        left_paddle.setPosition(L_PADDLE_X_POS, l_paddle_y_pos);
        right_paddle.setPosition(R_PADDLE_X_POS, r_paddle_y_pos);

        left_score.setString(std::to_string(l_points));
        right_score.setString(std::to_string(r_points));
                
        this->draw();

    }


    void end_game(bool left_player_won){

        sf::Text message;
        message.setFont(font);
        if(left_player_won)
            message.setString("Player on the Left won!!!");
        else 
            message.setString("Player on the right won!!!");
        message.setCharacterSize(24);
        message.setFillColor(sf::Color::White);
        message.setPosition(362 - 5, 50);

        game_window.clear();
        game_window.draw(message);
        game_window.display();
                    
        while(true)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                return;
            }
        }
    }

};



void initial_ball_speed(){
    velX = rand()%2 ? -0.2f : 0.2;
    velY = rand()%2 ? -0.2f : 0.2;
}


void wait_to_start(PongGame & game){

    bool start = false;

    while (!start)
    {
        // Handle events
        sf::Event event;
        while (game.game_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                game.game_window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                start = true;
                initial_ball_speed();
            }
        }

        // Clear the window
        game.game_window.clear();
        game.draw();

    }

}


#endif
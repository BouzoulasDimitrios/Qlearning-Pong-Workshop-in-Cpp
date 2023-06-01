
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

sf::Vector2f pos, l_paddle_pos, r_paddle_pos, ball_pos; 
sf::FloatRect left_paddleBounds, right_paddleBounds, ballBounds; 


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
        font.loadFromFile("./resources/arial.ttf");

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
    
    /**
     * resets game to starting state.
    */
    void reset(){
        
        ball_x = SCREEN_WIDTH/2 - BALL_RADIUS/2, ball_y = SCREEN_HEIGHT/2 - BALL_RADIUS/2;

        ball.setPosition(ball_x, ball_y);
        left_paddle.setPosition(L_PADDLE_X_POS, l_paddle_y_pos);
        right_paddle.setPosition(R_PADDLE_X_POS, r_paddle_y_pos);

        left_score.setString(std::to_string(l_points));
        right_score.setString(std::to_string(r_points));
                
        this->draw();

    }

    /**
     * end game sequence, writes message based on who won.
    */
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

        //wait untill enter is pressed
        while(true)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                return;
        
    }

};

/**
 * Randomizes initial ball speed/direction
*/
void initial_ball_speed(){
    velX = rand()%2 ? -0.5f : 0.5;
    velY = rand()%2 ? -0.5f : 0.5;
}

/**
 * User has to press enter in order to start the game
*/
void wait_to_start(PongGame & game){

    bool start = false;

    while (!start){

        sf::Event event;// Handle events
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

/**
 * Handles point scoring, resets the game.
*/
void point_scrored(PongGame &pg, int game_mode, float & reward, float ball_x_position){

    if (ball_x_position < 0.f){

        r_points += 1;       
        
        if(r_points == 10 && game_mode <= 2){                
            l_points = 0, r_points = 0;
            pg.end_game(false);
        }

        velX = -velX;
        pg.left_score.setString(std::to_string(r_points));
        pg.reset();
        pos = pg.ball.getPosition();        
        initial_ball_speed();
        reward = -1;

    }else if(ball_x_position > pg.game_window.getSize().x - pg.ball.getRadius() * 2.f){

        l_points += 1;
        if(l_points == 10 && game_mode <= 2){
            pg.end_game(true);
            l_points = 0, r_points = 0;
        }
            
        velX = -velX;
        pg.right_score.setString(std::to_string(l_points));
        pg.reset();
        pos = pg.ball.getPosition();
        initial_ball_speed();
        reward = 1;
    }
}

/**
 * calculates a correct move based on a given game state.
*/
void perfect_player(PongGame & pg){

    int i = rand()%2 ? -1 : 1 ;

    if(i == -1 && l_paddle_pos.y - PADDLE_MOVE > 0)
        pg.right_paddle.setPosition(r_paddle_pos.x , r_paddle_pos.y - 30);
    else if(i == 1 && r_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE)
        pg.right_paddle.setPosition(r_paddle_pos.x , pos.y + 22);

}

/**
 * Handling of user input during game, 
 * allows for paddle control based on game mode
 * for game mode 1 both user are allowed to control the paddles
 * for game mode 2 only the right side user can control the paddle.
*/
void handle_keypress(PongGame & pg, sf::Event event, int game_mode){

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

#endif
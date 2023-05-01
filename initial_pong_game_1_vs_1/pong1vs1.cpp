#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//$ g++ pong1vs1.cpp -o myapp -lsfml-graphics -lsfml-window -lsfml-system && ./myapp
#include <cmath>
#include <string>
#include <iostream>
#include "../resources/pong.h"
using namespace std;



int main()
{
    PongGame pg;
    pg.game_window.setKeyRepeatEnabled(false);

    sf::Vector2f pos;
    sf::Vector2f l_paddle_pos;
    sf::Vector2f r_paddle_pos; 
    
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
                
                l_paddle_pos = pg.left_paddle.getPosition();
                r_paddle_pos = pg.right_paddle.getPosition();

                if(event.key.code == sf::Keyboard::W && l_paddle_pos.y - PADDLE_MOVE > 0){
                    pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y - PADDLE_MOVE);
                }
                else if(event.key.code == sf::Keyboard::S && l_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE){
                    pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y + PADDLE_MOVE);
                }
                else if(event.key.code == sf::Keyboard::Up && r_paddle_pos.y - PADDLE_MOVE > 0 ){
                    pg.right_paddle.setPosition(r_paddle_pos.x, r_paddle_pos.y - PADDLE_MOVE);
                }
                else if(event.key.code == sf::Keyboard::Down && r_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE){
                    pg.right_paddle.setPosition(r_paddle_pos.x, r_paddle_pos.y + PADDLE_MOVE);
                }

            }

        }

        pos = pg.ball.getPosition();
        l_paddle_pos = pg.left_paddle.getPosition();
        r_paddle_pos = pg.right_paddle.getPosition();

        pos.x += velX;
        pos.y += velY;

        if (pos.x < 0.f)
        {
            r_points += 1;       
            
            if(r_points == 2)
                pg.end_game(false);

            velX = -velX;
            pg.left_score.setString(to_string(r_points));
            cout<<"+ 1 to the right player "<<r_points<<endl;
            pg.reset();
            pos = pg.ball.getPosition();        
            initial_ball_speed();

        }
        else if(pos.x > pg.game_window.getSize().x - pg.ball.getRadius() * 2.f){

            l_points += 1;
            if(l_points == 2)
                pg.end_game(true);

            velX = -velX;
            cout<<"+ 1 to the left player "<<l_points<<endl;
            pg.right_score.setString(to_string(l_points));
            pg.reset();
            pos = pg.ball.getPosition();
            initial_ball_speed();

        }   

        if (pos.y < 0.f || pos.y > pg.game_window.getSize().y - pg.ball.getRadius() * 2.f) { velY = -velY; } 
        

        sf::FloatRect left_paddleBounds = pg.left_paddle.getGlobalBounds();
        sf::FloatRect right_paddleBounds = pg.right_paddle.getGlobalBounds();
        sf::FloatRect ballBounds = pg.ball.getGlobalBounds();

        if ( left_paddleBounds.intersects(ballBounds) ) {
            cout<<"left colision"<<endl;            
            float t = ((pos.y - l_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = fabs(velX) + 0.02;
            velY = t*0.5;

        }else if( right_paddleBounds.intersects(ballBounds) ){
            cout<<"left colision"<<endl;
            float t = ((pos.y - r_paddle_pos.y)/ PADDLE_HEIGHT) - 0.5f;
            velX = -fabs(velX) - 0.02;
            velY = t*0.5;
        }

        pg.ball.setPosition(pos);
        pg.draw();
        pg.game_window.display();  

    }

    return 0;
}



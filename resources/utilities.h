#include <string>
#include <iostream>

/**
 * check that user input is a number
*/
bool check_number(std::string str) {
   for (int i = 0; i < str.length(); i++)
        if (isdigit(str[i]) == false)
            return false;
    return true;
}

/**
 * Function responsible for game mode selection, only accepts correct entries.
*/
int game_mode_selection(){

    std::string user_input;
    int mode = -1;
    std::cout << "Enter game mode:\n1) Player vs Player \n2)Player vs AI \n3) AI vs Hardcoded player \n4) train the model"<<std::endl; // Type a number and press enter
    std::cin >>user_input;
    if(check_number(user_input))
        mode = stoi(user_input);

    while(mode<1 || mode>4){
        std::cout << "Wrong entry. \n"; // Type a number and press enter
        std::cout << "Enter game mode:\n1) Player vs Player \n2)Player vs AI \n3) AI vs Hardcoded player \n4) train the model"<<std::endl; // Type a number and press enter
        std::cin >>user_input;
        if(check_number(user_input))
            mode = stoi(user_input);
    } 

    if(mode == 1)
        std::cout<<"Left Player controls: 's' UP  'w' DOWN \n Right Player controls: 'UP arrow' UP  'DOWN arrow' DOWN "<<std::endl;
    else if(mode == 2)
        std::cout<<"You control the right paddle, Player controls: 'UP arrow' = UP  'DOWN arrow' = DOWN "<<std::endl;
    else if(mode == 3)
        std::cout<<"Watch the Qlearning algorithm play against a perfect hardcoded player";

    std::cout<< "Please, press any Key to start the game \n";

    return mode;

}




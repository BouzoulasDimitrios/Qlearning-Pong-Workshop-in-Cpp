#include <string>
#include <iostream>

void print_table(std::vector<std::vector<float>> &Qtable){

    for(int  i = 0 ; i < 10 ; i++){ 
        for(int j = 0; j < Qtable[i].size(); j++){
            std::cout<<Qtable[i][j];
        }
        std::cout<<"\n";
    }
}

void save_table(std::string filename, std::vector<std::vector<float>> &Qtable){
    
    std::ofstream file(filename);
    for (const auto& row : Qtable) {
        for (const auto& qvalue : row) {
            file << qvalue << " ";
        }
        file << std::endl;
    }
    file.close();
}

void load_table(std::string filename, std::vector<std::vector<float>> &Qtable){

    std::ifstream file(filename);
    for (auto& row : Qtable) {
        for (auto& qvalue : row) {
            file >> qvalue;
        }
    }
    file.close();

}

/**
 * returns game action based on state of the game
 * earlier in the game actions are random as the exploration stage takes place
 * later the actions are based more and more on the qtables values
*/
int select_action(int game_state, std::vector<std::vector<float>> &Qtable, float exploration_rate){
    int action;

    // int random_value = static_cast <int> (rand()) / (static_cast <float> (RAND_MAX/1.0));
    // if(random_value > exploration_rate){

        // action = static_cast<float> (*max_element(std::begin(Qtable[game_state]), std::end(Qtable[game_state])));
        action = (*max_element(std::begin(Qtable[game_state]), std::end(Qtable[game_state])));
        std::cout<<"ACTION  SELECTED = "<<action<< " 1  =" <<Qtable[game_state][0]<< "1 = " <<Qtable[game_state][1]<< "2 = " <<Qtable[game_state][2] <<std::endl;
        // int  x = sizeof(Qtable[game_state]) / sizeof(int);
        // std::cout<<std::distance(Qtable[game_state], Qtable[game_state] + x)<<std::endl;
        
        // std::vector<int> v = {2, 1, 3, 6, 7, 9, 8};
    
        auto it = std::minmax_element(Qtable[game_state].begin(), Qtable[game_state].end());
        int min_idx = std::distance(Qtable[game_state].begin(), it.first);
        int max_idx = std::distance(Qtable[game_state].begin(), it.second);
        return max_idx;
    // }else{
    //     action = rand() % 3;
    //     std::cout<<"ACTION  RANDOM = "<<action<<std::endl;

    // }
    return action;

}

struct State {
  float paddle1, paddle2; // paddle positions
  float ball_x, ball_y;   // ball position
};


void update_qtable(std::vector<std::vector<float>> &Qtable, int game_state_end, int game_state_start, float learning_rate, float discount_rate, float reward, int action ){
    int next_move = static_cast<int> (*max_element(std::begin(Qtable[game_state_end]), std::end(Qtable[game_state_end])));
    Qtable[game_state_start][action] = Qtable[game_state_start][action] * (1 - learning_rate) + learning_rate * (reward + discount_rate * next_move);
}

void take_action(PongGame & pg, int action, int QPADDLE_MOVE){

    if( action == 0 && l_paddle_pos.y - QPADDLE_MOVE > 0 )
        pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y - QPADDLE_MOVE);
    else if( action == 2 && l_paddle_pos.y < SCREEN_HEIGHT - PADDLE_HEIGHT - QPADDLE_MOVE)
        pg.left_paddle.setPosition(l_paddle_pos.x, l_paddle_pos.y + QPADDLE_MOVE);
    else 
        return;
}

void init_state_table(std::vector<std::vector<int>> &lookupTable, int tableSize){

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

int correct_move_calculator(struct State state1, float vel){

    if(vel >= 0)// ball going down
        if(state1.paddle1 + 28 > state1.ball_y)
            return 1;
        else if(state1.paddle1 + 28 < state1.ball_y)
            return 2;
    else
        if(state1.paddle1 + 28 > state1.ball_y)
            return 0;
        else if(state1.paddle1 + 28 < state1.ball_y)
            return 1;

    return 0;

}

int state_calc(std::vector<std::vector<int>> & lookupTable, struct State & state, float velocity, std::size_t range){

    // std::cout<<"ball pad "<<state.ball_y<< "   " <<state.paddle1<<endl;
    // std::cout<<"State:  "<<lookupTable[state.ball_y][state.paddle1]<< "     "<<velocity<<endl;

    if(velocity > 0){
        // cout<<"POSTITIVE"<<velocity<<endl; //state.ball_x, 
        return range + lookupTable[state.ball_y][state.paddle1];     //5000 + state.ball_x + 2*state.ball_y + state.paddle1;
    }else{
        // cout<<"NEGATIVE"<<velocity<<endl; //state.ball_x, 
        return range - lookupTable[state.ball_y][state.paddle1];     //5000 - state.ball_x + 2*state.ball_y + state.paddle1;
    }

    return lookupTable[state.ball_y][state.paddle1];

}
#include <iostream>
#include <tuple>

std::tuple<int, int>  test (){



    return std::make_tuple( 321 , 2);
}



int main(){

    int i, j;
    std::tie(i, j) = test();
    std::cout<<"i = "<< i << " j = "<< j;


    return 0;
}


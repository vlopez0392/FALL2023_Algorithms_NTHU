/*
    FALL 2023 CS431102 Bonus programming Assignment 3
    Student: Victor D. Lopez 
    ID: 110062426
    //COMPILE WITH: g++ -Wall -std=c++11 -o bonus3 110062426_bonus3.cpp

    //EXECUTE WITH: ./bonus3 [n] - where n is the size of the problem

    //TIME COMPLEXITY - O(n!)

    //SPACE COMPLEXITY - Dominated by board size O(n)
*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <limits>

bool tryPlace(std::vector<int>& board, int row, int col){
    for(int i = 0; i < row; i++){
        if(board[i] == col || board[i] - i == col - row || board[i] + i == col + row){
            return false;
        }
    }
    return true;
}

void queenCounter(std::vector<int>& board, int n, int row, int& result){
    if(row == n){
        result++;
        return;
    }
    
    for(int j = 0; j < n; j++){
        if(tryPlace(board,row,j)){
            board[row] = j;
            queenCounter(board, n, row + 1, result);
            board[row] = 0;
        }
    }
}

int main(int argc, char *argv[]){
    if(argv[1] == nullptr){
        std::cout << "Value of n, not entered. Please execute as ./bonus3 n " << std::endl;
    }else{
        int n = std::atoi(argv[1]);
        std::vector<int> board = std::vector<int>(n, 0);
        int result = 0;
        queenCounter(board, n, 0, result);
        std::cout << "Solution for the n = " << n << " queen problem is: " << result << '\n';
        std::cout << '\n';

    }
}


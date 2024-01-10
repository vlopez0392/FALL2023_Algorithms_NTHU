/*
    FALL 2023 CS431102 Bonus programming Assignment 2
    Student: Victor D. Lopez 
    ID: 110062426
*/
//COMPILE WITH: g++ -Wall -std=c++11 -o bonus2 110062426_bonus2.cpp

#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <limits>

//Helper function
//Prints a pretty vector
void prettyPrintVector(std::vector<double> input, int n, int begin){
    for(int i = begin ; i < n; i++){
        if(i == begin){
            if(i == n-2){
                std::cout << "[ " << input[i] << " ]";
                break;
            }else{
                std::cout << "[ " << input[i] << ", ";
            }
        }else{
            if(i == n-2){
                std::cout << input[i] << " ]";
                break;
            }else{
                std::cout << input[i] << ", ";
            }
        }
    }
    std::cout<<'\n';
}

//Print pretty table
void printPrettyTable(std::vector<std::vector<double>>& table, int n, int beginRow, int beginColumn){
    for(int i = beginRow; i < n ; i++){
        prettyPrintVector(table.at(i), n, beginColumn);
        beginColumn++;
    }
}

//Begin computing the optimal value of the OPT-BST 
//Create tables e,w and r
void optimalBST(int n, std::vector<double>& p, std::vector<double>& q, std::vector<std::vector<double>>& e, 
                std::vector<std::vector<double>>& w, std::vector<std::vector<double>>& root){

    //Base cases for e(i,i-1);
    for(int i = 1; i<n+2; i++){
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    } 
    int j = 0;
    for(int l = 1; l<n+1;l++){
        for(int i = 1; i<n-l+1;i++){
            j = i+l-1;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            e[i][j] = std::numeric_limits<double>::max();

            double t = 0;
            for(int r = i; r < j+1; r++){
                t = e[i][r-1] + e[r+1][j] + w[i][j];

                if(t < e[i][j]){
                    e[i][j] = t;
                    root[i][j] = r;
                } 
            }
        }
    }
}

void printOptimalBSTStructure(std::vector<std::vector<double>>& root, int i, int j, int prev){
    if(root[i][j]>0){
        if(i > j){
            return;
        }
        if(prev == 0){
            std::cout<< "k" << root[i][j] << " is the root \n" ;
        }else if(j < prev){
            std::cout<< "k" << root[i][j] << " is the left child of " << "k" << prev << '\n';
        }else{
            std::cout<< "k" << root[i][j] << " is the right child of " << "k" << prev << '\n';
        }
        printOptimalBSTStructure(root, i, root[i][j]-1, root[i][j]);
        printOptimalBSTStructure(root, root[i][j]+1, j, root[i][j]);
    }
}

int main(int argc, char *argv[]){
    //---------------------------------INPUTS---------------------------------// 
    //Input pi and qi values given in assignment, also in problem 14.5-2
                            // 0     1     2    3     4     5     6     7     
    std::vector<double> p = {0.00, 0.04, 0.06, 0.08, 0.02, 0.10, 0.12, 0.14};
    std::vector<double> q = {0.06, 0.06, 0.06, 0.06, 0.05, 0.05, 0.05, 0.05};

    //Input pi and qi values in book example to verify results
                          //   0     1     2     3     4     5    
    // std::vector<double> p = {0.00, 0.15, 0.10, 0.05,  0.10,  0.20 };
    // std::vector<double> q = {0.05, 0.10, 0.05, 0.05,  0.05,  0.10 };
    //-----------------------------------------------------------------------// 

    //Compute optimal BST value
    int n = p.size(); //Compute number of keys  

    //Create e,w and r tables
    std::vector<std::vector<double>> e(n+2, std::vector<double> (n+1, 0)); 
    std::vector<std::vector<double>> w(n+2, std::vector<double> (n+1, 0)); 
    std::vector<std::vector<double>> root(n+1, std::vector<double> (n,0)); 

    //Compute optimal cost
    optimalBST(n,p,q,e,w,root);
    
    //---------------------------------OUTPUT---------------------------------//
    std::cout<< "----------------- Keys p and dummy keys q: -----------------" << '\n';
    std::cout<< '\n';
    std::cout<< "p: " ; 
    prettyPrintVector(p,n,1);
    std::cout<< "q: " ; 
    prettyPrintVector(q,n,0);
    std::cout<< '\n';
    
    std::cout<< "----------------- e table: -----------------" << '\n';
    std::cout<< '\n';

    printPrettyTable(e,n+1,1,0);
    std::cout<< '\n';
    std::cout<< "Optimal cost: " << e[1][n-1] << "\n \n";
    
    std::cout<< "----------------- root table: -----------------" << '\n';
    printPrettyTable(root,n+1,1,0);
    std::cout<< '\n';
    
    std::cout<< "----------------- OPT BST structure: -----------------" << '\n';
    int i = 1, j = n-1;
    int prev = 0;
    printOptimalBSTStructure(root,i,j,prev);
    std::cout<< '\n';
    std::cout<< "Dummy keys di-1 and di are the children of any key ki who doesn't have any key children. \n";
    return 0;
}
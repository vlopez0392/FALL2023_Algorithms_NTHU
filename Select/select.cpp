/*
    FALL 2023 CS431102 Bonus programming Assignment 1
    Student: Victor D. Lopez 
*/
#include <iostream>
#include <vector>
#include <stdio.h>
#include <random> 
#include <ctime>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>

//Helper functions
//Prints a pretty vector
void prettyPrintVector(std::vector<int> input, bool printAll, int limit = -1){
    int upperLimit;
    if(printAll){
        upperLimit = input.size();
    }else{
        upperLimit = limit;
    }

    if(upperLimit == 1){ //Size 1 input array
        std::cout << "[ " << input[0] << " ]";
    }else{
        for(int i = 0 ; i < upperLimit; i++){
            if(i == 0){
                std::cout << "[ " << input[i] << ", ";
            }else{
                if(i == upperLimit-1){
                    std::cout << input[i] << " ]";
                }else{
                    std::cout << input[i] << ", ";
                }
            }
        }
    }
    std::cout<<'\n';
}

//Swap 
void swap(std::vector<int>& inputArray, int i, int j){
    if(i >= 0 && j >= 0 && i < inputArray.size() && j < inputArray.size()){
        int temp = inputArray[i];
        inputArray[i] = inputArray[j];
        inputArray[j] = temp;
    }
}

//Populate the vector with random numbers
std::vector<int> createRandomVector(int N, int size){
    std::vector<int> randomVector(size);

    for(int i = 0; i < randomVector.size(); i++){
        randomVector[i] = rand()%(N)+1;
    } 
    return randomVector;
}

//Generate index array
std::vector<int> generateIndexArray(int start, int size){
    std::vector<int> indexArray;
    for(int i = start; i < size ; i++) indexArray.emplace_back(i);
    return indexArray;
}

//End of helper functions

//SELECT algorithm components
//Insertion sort for each group - Modified to save index movements
void insertionSort(std::vector<int> &inputArray, std::vector<int> &indexArray){
    int i = 0, key = 0, idxKey = 0;

    for(int j = 1; j < inputArray.size(); j++){
        key = inputArray[j];
        idxKey = indexArray[j];
        i = j - 1;

        while(i >= 0 && inputArray[i] > key){
            inputArray[i+1] = inputArray[i]; 
            indexArray[i+1] = indexArray[i]; //Index follows element's position
            i = i-1;
        }
        inputArray[i+1] = key;
        indexArray[i+1] = idxKey;
    }
}

//Get median and it's index - Array most be sorted beforehand
std::tuple<int,int> getMedian(std::vector<int>& inputArray, std::vector<int>& indexArray){
    int N = inputArray.size();
    if(N == 1){
        return {indexArray[0],inputArray[0]};
    }else{
        if(N%2 == 0){
            return {indexArray[N/2-1], inputArray[N/2 - 1]};
        }else{
            return {indexArray[N/2], inputArray[N/2]};
        }
    }
}

//Divide elements into groups of G elements and return median of medians
std::tuple<int,int> medianFinder(int G, std::vector<int> inputArray, std::vector<int> indexArray){
    std::tuple<int,int> m = {0,0};

    //We have recursed to a level where the median of medians array size <= G, return median of sorted array 
    if(inputArray.size() <= G){
        insertionSort(inputArray, indexArray);
        m = getMedian(inputArray,indexArray);
       return m;
    }else{ //Divide groups in sizes N/G 
        int count = 0, start = 0, end = G;
        bool flagBreak = false;
        std::vector<int> temp, tempIdx, medians, medianIdx;
        
        while(true){
            for(int i = start; i < end; i++){
                temp.emplace_back(inputArray[i]);
                tempIdx.emplace_back(indexArray[i]);
            }

            //Sort each array group
            insertionSort(temp, tempIdx); 

            //Get the median and its index and add it to the arrays
            m = getMedian(temp, tempIdx);
            medianIdx.emplace_back(std::get<0>(m));
            medians.emplace_back(std::get<1>(m));

            //Update indexes for next group
            count+=1;
            start = G*count;
            end = start + G;
            temp.clear();
            tempIdx.clear();

            if(end >= inputArray.size()){
                end = inputArray.size();
                if(!flagBreak){
                    flagBreak = true;
                }else{
                    break;
                }
            }
        }
        //Recurse to find median of medians
        m = medianFinder(G,medians, medianIdx);
    }
    return m;
}

//Partition 
int partition(std::vector<int>& inputArray, std::vector<int>& indexArray, int p, int r, std::tuple<int,int> m){
    r += 1;
    
    //Make median last element r and proceed with partition normally
    std::tuple<int,int> last = std::make_tuple(indexArray[r-1], inputArray[r-1]);
    inputArray[r-1] = std::get<1>(m);
    indexArray[r-1] = std::get<0>(m);

    inputArray[std::get<0>(m)] = std::get<1>(last);
    indexArray[std::get<0>(m)] = std::get<0>(last);

    //Begin partition in place
    int i = p-1;
    int x = inputArray[r-1];

    for(int j = p; j < r-1; j++){
        if(inputArray[j] <= x){
            i = i+1;
            swap(inputArray,i,j);
            swap(indexArray,i,j);
        }
    }
    swap(inputArray,i+1,r-1);
    swap(indexArray,i+1,r-1);
    return i+1;
}

int select(std::vector<int>& inputArray, std::vector<int>& indexArray, 
    std::vector<int> A, std::vector<int> idxA, int G, int N, int K, int p, int r){
    if(p == r){
        std::cout << "Select returned the kth element: " << inputArray[p] << '\n' ;
        return inputArray[p];
    }

    //Find median of medians
    std::tuple<int, int> m = medianFinder(G, A, idxA); 

    //Partition array in place 
    int q = partition(inputArray, indexArray,p, r, m); //Returns number of elements less than m (index of m) 
    int i = q-p+1;
    
    if(K == i){
        std::cout << "Select returned the kth element: " << inputArray[q] << '\n' ;
        return inputArray[q];
    }else{ //Subproblems 
        if(K < i){
            r = q-1;
        }else{
            p = q+1;
        }
        
        //Some bookeeping for the medianFinderAlgorithm
        std::vector<int> subA = std::vector<int>(inputArray.begin()+p,inputArray.begin()+r+1);
        std::vector<int> subIdxA = generateIndexArray(p,r+1);
        
        if(K < i){
            return select(inputArray, indexArray, subA, subIdxA,G,N,K,p,r);
        }else{
            return select(inputArray, indexArray, subA, subIdxA,G,N,K-i,p,r);
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    //Create random seed
    srand((unsigned int)time(NULL));

    //FILE IO
    //Input
    //Read data from input.txt file 
    std::vector<std::vector<int>> input; //<[N,G,K]>
    std::ifstream file_in("input.txt");
    if (!file_in) {
        std::cout << "input.txt not found" << '\n';
    }else{
        std::string line;
        while (std::getline(file_in, line))
        {
            std::istringstream ss(line);
            input.emplace_back(std::istream_iterator<int>(ss), std::istream_iterator<int>());
        } 
    }
    int N = input[0][0];
    int K = input[0][1];
    int G = input[0][2];
    
    //Begin measuring time
    auto beg = std::chrono::high_resolution_clock::now();

    std::vector<int> inputVec = createRandomVector(N, N);
    std::vector<int> indexArray = generateIndexArray(0,N);
    std::vector<int> copyInputVec = inputVec;
    
    //Select 
    int k = select(inputVec, indexArray, inputVec, indexArray, G, N, K, 0, N-1);
        
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
    std::cout << "Time elapsed in seconds: " << duration.count() << '\n';   

    //Check result by sorting a copy of the input vector
    std::sort(copyInputVec .begin(), copyInputVec.end());
    std::cout << "Kth element checked by sorting: " <<copyInputVec[K-1] << '\n';
    std::cout << '\n'; 

    //FILE IO - Write results to output.txt
    std::ofstream outputfile;
    outputfile.open("output.txt");
    outputfile << k << '\n'; //Output newline after number
    outputfile.close();
    
    return 0;
}
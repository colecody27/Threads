#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <cstring>
#include <queue>
#include <unordered_map>
using namespace std;


//Global variables 
queue<string> words;
unordered_map<string, bool> um; 
bool number = false;


bool isNumber(string word){
    char letter = word[0];
    if(letter >= 48 && letter <= 57){
        return true;
    }
    else
        return false;
}

//Functions called by threads
void *alpha(void * arg){
    char * name = (char *)arg;
    string token1 = name;

    //Iterate through words 
    while(words.size() > 0){
        string token2 = words.front();

        //Check character to see if it is in alphabet
        if(!um.at(token2)){
            printf("%s: %s\n", token1.c_str(), token2.c_str());
            words.pop();
        }else
            number = true;
    }
    pthread_exit(0);
}

void *numeric(void * arg){
    char * name = (char *)arg;
    string token1 = name;

    //Iterate through words 
    while(words.size() > 0){
        string token2 = words.front();

        //Check token2 type in um
        if(um.at(token2) && number){
            printf("%s: %s\n", token1.c_str(), token2.c_str());
            words.pop();
            number = false;
        }
    }
    pthread_exit(0);
}

//Main function 
int main(int argc, char* argv[]){

    //Check for valid input
    if(argc < 2){
        fprintf(stderr, "Not enough arguments");
        exit(-1);
    }

    //Split string into vector of strings
    char *ptr = strtok(argv[1], " ");
    int wordCount = 0;
    while(ptr != NULL){ 
        //Insert words into hashmap 
        string word = ptr;

        //Add word to um
        if(um.find(word) == um.end())
            um[word] = isNumber(word);
        
        //Add word to queue 
        words.push(ptr);
        ptr = strtok(NULL, " ");
    }

    //Create threads
    pthread_t pA, pN;
    
    char * msgA = (char *)"alpha";
    char * msgB = (char *)"numeric";

    pthread_create(&pA, NULL, alpha, (void *)msgA);
    pthread_create(&pN, NULL, numeric, (void *)msgB);

    pthread_join(pA, NULL);
    pthread_join(pN, NULL);
    return 0;
}
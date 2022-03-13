#include <iostream>
#include <fstream>

#include "HashTable.h"

const int MAX_WORD_LIMIT = 10000;

using namespace std;

//Capitalises a passed string
void capitaliseString(string &word){
    for(int i = 0; i<word.length();i++){
        word[i] = toupper(word[i]);
    }
}

//Checks to see what type fo grams the user wants to save
int ngramType(string keyWord){
    capitaliseString(keyWord);
    if(keyWord == "WORD"){
        return 1;
    }
    if(keyWord == "CHAR"){
        return 2;
    }
    if(keyWord == "DECIMAL"){
        return 3;
    }
    cout<<"Keyword not accepted. Please insert word, char or decimal."<<endl;
    throw 4;
}

//Displays the frequency of the most frequent items in the table
template <class KeyType, class ValueType>
int frequencyAnalysis(HashTable<KeyType,ValueType> *table, int displayNo){
    int numberOfFullNode=0;
    //Check how many slots of the table are full
    for(int i =0; i < table->size(); i++){
        if(table->grabNode(i)->getUsed()){
            numberOfFullNode++;
        }
    }
    //If there isn't enough items to display then return error message
    if(displayNo>numberOfFullNode){
        cout<<"There isn't enough nodes to have a top "<< displayNo << " displayed."<<endl;
        return 1;
    }
    cout<<"Analysing Data"<<endl;
    int maxNodePosition = 0;
    int numberOfLetters = 0;
    int tempInt;
    float tempFloat;
    string tempPercentage;
    string tempString; 

    //Loop for the amout of items I want to display
    for(int counter = 0; counter < displayNo; counter++){
        //Find the first item in the table that could be the max
        for(int i =0; i < table->size(); i++){
            if(!table->grabNode(i)->getBeenMax() && table->grabNode(maxNodePosition)->getUsed() && table->grabNode(maxNodePosition)->getOccurances()!=0){
                maxNodePosition = i;
                break;
            }
        }
        //Find the max that hasn't been used yet
        for(int i =0; i < table->size(); i++){
            if(table->grabNode(i)->getOccurances() >= table->grabNode(maxNodePosition)->getOccurances() && !table->grabNode(i)->getBeenMax() && table->grabNode(i)->getUsed()&& table->grabNode(i)->getOccurances()!=0){
                maxNodePosition = i;
            }
        }
        tempPercentage ="";
        tempString = "";
        //Say that this value has now been a max
        table->grabNode(maxNodePosition)->setBeenMax(true);
        //Find out the percentage
        tempFloat = table->grabNode(maxNodePosition)->getOccurances();
        tempFloat = (tempFloat / table->getNum())*100;
        tempInt = tempFloat;
        //Find how many characters are needed
        //if it is an integer (so no need for decimal places)
        if(tempFloat == tempInt){
            if(tempFloat>=10){numberOfLetters=2;}
            else if(tempFloat==100){numberOfLetters=3;}
            else{numberOfLetters=1;}
        }
        else{
            if(tempFloat>=10){numberOfLetters=5;}
            else if(tempFloat==100){numberOfLetters=6;}
            else{numberOfLetters=4;}
        }
        //Convert the percentage to be to 2 decimal places
        stringstream ss; 
        ss.str("");
        ss<<tempFloat;  
        ss>>tempString;
        for(int iterator = 0; iterator < numberOfLetters; iterator++){
            tempPercentage += tempString[iterator];
        }
        cout<<tempPercentage<<":"<<table->grabNode(maxNodePosition)->getKey()<<endl;

        //Print Data
        // cout<< "In "<<counter+1;
        // if(counter==0){cout<<"st";}
        // else if(counter==1){cout<<"nd";}
        // else if(counter==2){cout<<"rd";}
        // else{cout<<"th";}
        //cout<<" place is '"<<table->grabNode(maxNodePosition)->getKey()<<"' with "<< table->grabNode(maxNodePosition)->getOccurances()<<" occurances; which is " << tempPercentage << "% of the ngrams." <<endl;
    }
    //Reset table since I'm done with it.
    for(int i =0; i < table->size(); i++){
        table->grabNode(i)->setBeenMax(false);
    }
}

//Save word ngrams to a hash table
void saveWords(string &wholeThing, int ngramLength, int numberOfWinners){
    HashTable<string,int> tempTable;
    HashTable<string,int> *table = &tempTable;
    int i, counter =0;
    string word, ngram;
    string wordArray[MAX_WORD_LIMIT];
    string delimiter = " ";
    int delimiterPlace = 0;

    //While the delimiter can be found
    while ((delimiterPlace = wholeThing.find(delimiter)) != string::npos) {
        //Set word to the whole thing up to where the next space (delimiter) is.
        word = wholeThing.substr(0, delimiterPlace);
        wordArray[counter] = word;
        //Erase up to and including the delimiter
        wholeThing.erase(0, delimiterPlace + 1);
        counter++;
    }

    //For all the groups avaliable
    for(int i = 0; wordArray[i+ngramLength-1]!=""; i++){
        //reset ngram
        ngram = "";
        //Add all the words required to the ngram
        for(int counter=0; counter<ngramLength; counter++){
            ngram += wordArray[i+counter];
            //Add a space unless it is the last word
            if(counter<ngramLength-1){
                ngram += " ";
            }
        }
        //save the ngram
        table->insert(ngram,0);
    }
    frequencyAnalysis(table,numberOfWinners);
}

//Save char ngrams to a hash table
void saveChars(string wholeThing, int ngramLength, int numberOfWinners){
    HashTable<string,int> tempTable;
    HashTable<string,int> *table = &tempTable;
    string ngram;
    //For all the places that could have n grams
    for(int i = 0; i<wholeThing.length()-ngramLength; i++){
        //reset ngram
        ngram = "";
        //Save ngram to ngram one char at a time
        for(int counter=0; counter<ngramLength; counter++){
            ngram += wholeThing[i+counter];
        }
        table->insert(ngram,0);
    }
    frequencyAnalysis(table,numberOfWinners);
}

//Save decimal ngrams to a hash table. Basically the same as saving chars but tries converting to float before inserting into table
int saveDecimal(string wholeThing, int ngramLength, int numberOfWinners){
    HashTable<float,int> tempTable;
    HashTable<float,int> *table = &tempTable;
    string ngram;
    for(int i = 0; i<wholeThing.length()-ngramLength; i++){
        ngram = "";
        for(int counter=0; counter<ngramLength; counter++){
            ngram += wholeThing[i+counter];
        }
        try{
            table->insert(stof(ngram),0);
        }catch(invalid_argument){
            continue;
        }    
    }
    frequencyAnalysis(table,numberOfWinners);
}

int saveNgrams(string fileName, int ngramType, int ngramLength, int numberOfWinners){
    string line;
    string wholeThing = "";
    ifstream readFile(fileName, ios::in);
    if (!readFile){
        return 1;
    }
    //reads in file line by line saving to the string wholeThing
    while(getline(readFile, line)) {
        wholeThing += line + " ";
    }
    readFile.close();

    if(ngramType == 1){
        saveWords(wholeThing, ngramLength,numberOfWinners);
    }
    else if(ngramType == 2){
        saveChars(wholeThing, ngramLength,numberOfWinners);
    }
    else{
        saveChars(wholeThing, ngramLength,numberOfWinners);
    }
    return 0;
}

int properMain(string programName,string fileName,int ngramLength,int noOfWinners,string keyWord){
    int type = 0;
    try{
        type = ngramType(keyWord);
    }catch(...){
        return 1;
    }

    int tempInt = saveNgrams(fileName, type, ngramLength, noOfWinners);
    if(tempInt == 1){
        cout<<"The file given cannot be read"<<endl;
        return 1;
    }

   return 0;
}

int main(int argc, char *argv[]){
    // argv[1] = the name of the program;
    // argv[2] = fileName;
    // argv[3] = number of items for the ngram;
    // argv[4] = number of most frequent ngrams to display;
    // argv[5] = One of the keywords: word,char,decimal;

    //set default values for the variables
    string programName = "ngramFrequency";
    string fileName = "inputfile.txt";
    int ngramLength = 3;
    int noOfWinners = 10;
    string keyWord = "char";

    //Set the input variables to custom variables
    if(argc>=1){
        programName = argv[0];
        if(argc>=2){
            fileName = argv[1];
            if(argc>=3){
                ngramLength = atoi(argv[2]);
                if(ngramLength<1){
                    cout<<"Please insert a valid number of items you want per ngram"<<endl;
                    return 1;
                }
                if(argc>=4){
                    noOfWinners = atoi(argv[3]);
                    if(noOfWinners<1){
                        cout<<"Please insert a valid number of frequencies you want to display"<<endl;
                    return 1;
                    }
                    if(argc>=5){
                        keyWord = argv[4];
                    }
                }
            }
        }
    }
    //Run the program
    properMain(programName, fileName, ngramLength, noOfWinners,keyWord);
}
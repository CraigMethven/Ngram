#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "HashNode.h"
#include <sstream> 
#include <vector>
#include <string>

using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY, NOT_IN_TABLE };

template <class KeyType, class ValueType>
class HashTable {
  //Defines table as a vector of hashNodes that have the same data types as KeyType and ValueType.
  typedef vector <HashNode<KeyType,ValueType> > Table;
  //A pointer to the vector of hashNodes
  Table *table; // size of table (# of buckets) is stored in the Table data structure
  int num;   // number of entries stored in the HashTable;

public:
  HashTable();        // constructor, initializes table of size 11;
  HashTable(int);  // constructor, requires size of table as arg
  ~HashTable();       // deconstructor

  int size();      // returns size of the hash table (number of buckets)
  int getNum(){return num;}
  int hash_function(KeyType);  // the table's hash function
  ValueType getValue(KeyType);    // find and return data associated with key
  HashNode<KeyType,ValueType>* grabNode(int);       //returns a node given a position in the table

  void insert(KeyType,ValueType); // insert data associated with key into table
  void erase(KeyType);            // remove key and associated data from table

  void rehash(int); // sets a new size for the hash table, rehashes the hash table 
  void checkIfTableFull(); //checks to see if the table isfull to see if rehashing needs to be done

  void printTable();
};

//Constructor with default size 11
template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::HashTable() {
  table = new Table;
  //Sets the size
  table->resize(11);
  num = 0;
}

//Constructor with the size of the table given
template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::HashTable(int potato) {
  table = new Table;
  table->resize(potato);
  num = 0;
}

//Destructs Table
template <class KeyType, class ValueType>
HashTable<KeyType,ValueType>::~HashTable() {
  table->clear();
}

//Returns Size
template <class KeyType, class ValueType>
int HashTable<KeyType,ValueType>::size() {
  return table->size();
}

//Returns the integer that is the place in the vector that the key leads to.
template <class KeyType, class ValueType>
int HashTable<KeyType,ValueType>::hash_function(KeyType kt) {
  int counter =0;
  int x =0;
  string tempString;
  stringstream ss;  

  //clear the string stream then translate kt to a string
  ss.str("");
  ss<<kt;  
  ss>>tempString;  
  //Add a | character so we know when the end of the string has been reached
  tempString = tempString + "|";
  
  while (tempString[x] != '|'){ 
    //Add the ascii of each character in the string to the int counter
    counter += int(tempString[x]); 
    x++; 
  }
  //If counter is negative then make it positive
  if(counter<0){
    counter *= -1;
  } 
  //Make counter in range of the table
  counter = counter%size();
  return(counter);
}

//Gets the value stores in the node stored at the position passed in.
template <class KeyType, class ValueType>
ValueType HashTable<KeyType,ValueType>::getValue(KeyType KT) {
  int pointer = hash_function(KT);
  //for all slots in the table
  for(int i = 0; i<size(); i++){
    //Sees if the slot has been used before; if it hasn't then the item can't be in the table so throws a key not found error
    if(table->at(pointer).getUsed()){
      //If the node hasn't been deleted
      if(table->at(pointer).getOccurances()!=0){
        //if this is the node we want return value or occurances of that node
        if(table->at(pointer).getKey() == KT){       
          return table->at(pointer).getValue();
        }
      }
    }
    else{
      //NOT IN TABLE
      throw KEY_NOT_FOUND;
    }
    //Using pointer as the incrementing variable so that I can do wrapping
    pointer++;
    if(pointer >= size()){
      pointer -= size();
    }
  }
}

//returns a pointer to the node at the position passed in
template <class KeyType, class ValueType>
HashNode<KeyType,ValueType>* HashTable<KeyType,ValueType>::grabNode(int position){
  return &(table->at(position));
}

//checks to see if the table isfull to see if rehashing needs to be done
template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::checkIfTableFull(){
  if(num > (size()/2)){
    rehash((size()*4));
  }
}

//Inserts data into the table
template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::insert(KeyType key,ValueType value) {
  this->num = this->num + 1;

  int pointer = hash_function(key);
  //Checks all slots
  for(int i = 0; i<size(); i++){
    //If the node hasn't been used or has been deleted
    if(!table->at(pointer).getUsed() || table->at(pointer).getOccurances() == 0){
      //Inserts the node at this position then checks if the table is full
      table->at(pointer).assign(key,value);
      checkIfTableFull();
      return;
    }
    //If the node is in use but has the same key then increment the occurances on that node
    else if(table->at(pointer).getKey() == key){
      table->at(pointer).incrementOccurances();
      return;
    }
    pointer++;
    if(pointer >= size()){
      pointer -= size();
    }
  }
}

//removes a key (and all in it) from the table
template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::erase(KeyType key) {
  //Same sorta thing as get value except decrements occurances rather than returning value
  int pointer = hash_function(key);
  for(int i = 1; i<size(); i++){
    if(table->at(pointer).getUsed()){
      if(table->at(pointer).getKey() == key){
        table->at(pointer).decrementOccurances();
        this->num = this->num -1;
        return;
      }
    }
    else{
      throw KEY_NOT_FOUND;
    }
    pointer++;
    if(pointer >= size()){
      pointer -= size();
    }
  }
}

//rehashes the table to a new table of the size passed in
template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::rehash(int newSize) {
  //grabs the size of the current table
  int tempInt = size();

  //Setup the new table
  Table *newTable = new Table();
  newTable->resize(newSize);

  //Create an instance of the old table before setting the new one.
  Table *tempTable1 = table;
  table = newTable;

  //Loops for all nodes in the original table
  for(int i = 0; i < tempInt; i++){
    //If the node hasn't been used or has been deleted then skip it
    if(!tempTable1->at(i).getUsed() || tempTable1->at(i).getOccurances() == 0){
      continue;
    }
    //If there are multiple instances of the node in the table then add it that many times to the new table
    for(int counter=0; counter<tempTable1->at(i).getOccurances(); counter++){
      insert(tempTable1->at(i).getKey(), tempTable1->at(i).getValue());
    }
  }
}

//Prints the entire table
template <class KeyType, class ValueType>
void HashTable<KeyType,ValueType>::printTable() {
  for(int i = 0; i<size(); i++){
    cout<<"Position: "<< i << "\tKey: "<< table->at(i).getKey()<<"\tValue: "<<table->at(i).getValue()<<endl;
  }
}
#endif
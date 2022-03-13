#ifndef MY_HASH_NODE
#define MY_HASH_NODE

using namespace std;


//KeyType is the key code that is being used to find each node. ValueType is the values that go ito the node
template <class KeyType, class ValueType>
class HashNode {
  //KeyType is the type that key is. So integers for a table that uses integers as the key. These need to be defined when a hashNode is created. This is similar to ValueType
  KeyType key;     // The hash node's key
  ValueType value; // The key's associated data
  bool used;  //Boolean to see if the node has been used before
  int occurances; //Stores the amount of times the node appears
  bool beenMax; //Bool to store if this node has been the maximum before (used for finding max node)

  /* extend if necessary */

public:

  HashNode();  // constructor

  KeyType getKey() { return key; }
  ValueType getValue() { return value; }
  void assign(KeyType key, ValueType value); 
  bool getUsed(){ return used; }
  bool getBeenMax(){return beenMax;}
  void setBeenMax(bool tempBool){beenMax = tempBool;}
  int getOccurances(){ return occurances; }
  void setOccurances(int x){ occurances = x; };
  void incrementOccurances(){ occurances++; }
  void decrementOccurances(){ occurances--; }

  // extend if necessary
};


template <class KeyType, class ValueType>
HashNode<KeyType,ValueType>::HashNode() {
  used = false;
  occurances = 1;
  beenMax = false;
}

template <class KeyType, class ValueType>
void HashNode<KeyType,ValueType>::assign(KeyType key, ValueType value) {
  this->occurances = 1;
  this->used = true;
  this->key = key;
  this->value = value;
}

#endif

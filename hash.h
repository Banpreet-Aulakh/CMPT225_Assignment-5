#pragma once

#include <iostream>
#include <cstring>

using namespace std;


const unsigned scale = 225;


// * * * replace with your own personal modulus * * * // 3000 + 385 -> 3389
const unsigned M = 3389;



// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
    private:
        T * A[M];



        // * * * Add attributes and helper methods here if you wish. * * * //

        unsigned hash(const string key) const;
        int charConvert(const char c) const;
        int probe(const unsigned key, const unsigned probe) const;
        


    public:
        // Desc:  Default constructor.  Initialize table to NULL.
        Set() { 
            memset(A, 0, sizeof(T *)*M); 
        }


        // Desc:  Insert x into the Set, according to its ->key.
        //        If ->key is already present, then replace it by x.
        //        Collisions are resolved via quadratic probing sequence.
        // Post:  returns the table index where x was inserted
        //        returns -1 if x was not inserted
        int insert(T * x);


        // Desc:  Returns T * x such that x->key == key, if and only if 
        //        key is in the Set.
        //        Returns NULL if and only if key is not in the Set.
        // Post:  Set is unchanged
        T * search(string key) const;

};





// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T * x) {
   string key = x->key;
   unsigned index = hash(key);
   unsigned initial = index;
   int i = 0;
   while (A[index] != 0 && (key != A[index]->key)) {
      ++i;
      index = probe(initial, i);
      if (index == initial) {
         return -1;
      }
   }
   A[index] = x;
   return index;
}


// Desc:  Returns T * x such that x->key == key, if and only if 
//        key is in the Set.
//        Returns NULL if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T * Set<T>::search(string key) const {
   unsigned index = hash(key);
   unsigned initial = index;
   if (A[index] == 0) {
      return NULL;
   }
   else{
      int i = 0;
      while (A[index] != 0) {
         if (A[index]->key == key) {
            return A[index];
         }
         else {
            index = probe(index, ++i);
            if (index == initial) {
               return NULL;
            }
         }
      }
   }
   return NULL;
}

template<class T>
unsigned Set<T>::hash(const string key) const { // use horner's method to apply hashing function
   unsigned val = charConvert(key[0]) * scale; //to prevent overflow from the hash function
   if (key.size() == 1) {
      return val * 64 % M;
   }
   for (int i = 1; i < key.size(); ++i) {
      val = (val*64 + charConvert(key[i]) * scale) % M;
   }
   return val;
}


template<class T>
int Set<T>::charConvert(const char c) const{
   int ascii = int(c);
   if (ascii >= 48 && ascii <= 57) {
      ascii -= 48;
   }
   else if (ascii >= 65 && ascii <= 90) {
      ascii -= 29;
   }
   else if (ascii >= 97 && ascii <= 122) {
      ascii -= 87;
   }
   else if (ascii == 95) {
      ascii = 62;
   }
   else
      return 0;

   return ascii;
}

template<class T>
int Set<T>::probe(const unsigned key, const unsigned probe) const{ // adds quadratic probe to hash key
   return (key + (probe * probe));
}
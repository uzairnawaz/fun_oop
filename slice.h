#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

// A slice represents an immutable substring.
// Assumptions:
//      * the underlying string outlives the slice
//      * the underlying string is long enough
//      * the underlying string can be represented with single byte
//        characters (e.g. ASCII)
//
//      Slice representing "cde"
//          +---+---+
//          | o | 3 |
//          +-|-+---+
//            |
//            v
//       ...abcdefg...
//
// This class is intended as a light-weight wrapper around pointer and length
// and should be passed around by value.
//

typedef struct Slice {
    char const * start;  
    size_t len;         // How many characters in the string
} Slice;

bool slice_string_equals(Slice s, const char* p);
bool slice_equals(Slice s1, Slice s2);
bool slice_is_identifier(Slice s);
void slice_print(Slice s);
size_t slice_hash(Slice key);

/*
 Returns the hash value for a given slice.
 Defined as a struct to pass to a std data structure 
*/
typedef struct slice_hash_func {
   size_t operator()(Slice key) const {
        return slice_hash(key);
    }
} slice_hash_func;

/*
 Returns true if two slices contain the same characters, false otherwise.
 Defined as a struct to pass to a std data structure 
*/
typedef struct slice_equals_func {
    bool operator()(Slice s1, Slice s2) const {
        return slice_equals(s1, s2);
    }
} slice_equals_func;



#include "slice.h"

/*
 Returns true if the data in the slice matches the given string parameter, false otherwise
*/
bool slice_string_equals(Slice s, const char* p) {
    for (size_t i = 0; i < s.len; i++) {
        if (p[i] != s.start[i])
            return false;
        }
    return p[s.len] == 0; // checks if p and s are the same size
};

/*
 Returns true if two slices contain the same values
*/
bool slice_equals(Slice s1, Slice s2) {
    if (s1.len != s2.len)
        return false;
    for (size_t i = 0; i < s1.len; i++) {
        if (s1.start[i] != s2.start[i])
            return false;
    }
    return true;
}

/*
 Returns the hash value for a given slice
*/
size_t slice_hash(Slice key) {
    // djb2
    size_t out = 5381;
    for (size_t i = 0; i < key.len; i++) {
        char const c = key.start[i];
        out = out * 33 + c;
    }
    return out;
}

/*
 Returns true if a given slices is an identifier (contains only letters and numbers and starts with 
 a letter)
*/
bool slice_is_identifier(Slice s) {
    if (s.len == 0)
        return false;
    if (!isalpha(s.start[0]))
        return false;
    for (size_t i = 1; i < s.len; i++)
        if (!isalnum(s.start[i]))
           return false;
    return true;
}

/*
 Prints the contents of the slice to stdout
*/
void slice_print(Slice s) {
    for (size_t i = 0; i < s.len; i++) {
        printf("%c", s.start[i]);
    }
}


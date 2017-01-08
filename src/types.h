#pragma once

#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

using namespace std;

typedef enum type_e {
    NIL         = 0,
    BOOL        = 1,
    INT         = 2,
    FLOAT       = 3,
    STRING      = 4,
    ARRAY       = 5,
    FUNCTION    = 6
} type_t;

typedef union value_u {
    bool b; // boolean
    int i; // integer
    float f; // float
    string& s; // string
    vector<Type>& a; // array
} value_t;

class Type {
public:
    type_t T;
    value_t V;

    //bool checkCompatible(Type Other);
};

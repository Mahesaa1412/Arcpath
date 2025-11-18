#ifndef TURNBASE_H_INCLUDED
#define TURNBASE_H_INCLUDED
#include <iostream>
using namespace std;

typedef struct skillNode *adrNode;

struct skill{
    string namaSkill;
    int staminaCost;
    int damage;
};

struct skillNode {
    skill info;
    adrNode left;
    adrNode right;
};


#endif // TURNBASE_H_INCLUDED

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

void createTree(adrNode &root);
adrNode createNode(string namaSkill, int damage, int staminaCost);
void insertNode(adrNode &root, adrNode p);
void insertSkill(adrNode &root, string namaSkill, int damage, int staminaCost);
adrNode buildWarriorTree();
adrNode buildArcherTree();
adrNode buildMageTree();
void displaySkill(adrNode root);
adrNode searchSkill(adrNode root, string namaSkill);


#endif // TURNBASE_H_INCLUDED


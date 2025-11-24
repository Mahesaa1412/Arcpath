#ifndef TURNBASE_H_INCLUDED
#define TURNBASE_H_INCLUDED
#include <iostream>
using namespace std;

typedef struct skillNode *adrNode;

struct skill{
    int idSkill;
    string namaSkill;
    int staminaCost;
    int damage;
    int mana;
};

struct skillNode {
    skill info;
    adrNode left;
    adrNode right;
};

struct Atribut{
    int maxHp;
    int maxStamina;
    int maxMana;
};

Atribut warriorAtribut();
Atribut mageAtribut();
Atribut archerAtribut();
void karakter(Atribut player);

void displayPilihRole();
void createTree(adrNode &root);
adrNode createNode(int idSkill, string namaSkill, int damage, int staminaCost, int mana);
void insertNode(adrNode &root, adrNode p);
adrNode pilihRoleTree(int choice);
void insertSkill(adrNode &root, int idSkill, string namaSkill, int damage, int staminaCost, int mana);
adrNode buildWarriorTree();
adrNode buildArcherTree();
adrNode buildMageTree();
void displaySkill(adrNode root);
adrNode searchSkill(adrNode root, string namaSkill);
void postOrder(adrNode root);


#endif // TURNBASE_H_INCLUDED

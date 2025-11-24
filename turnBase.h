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

// Atribut Player Sesuai Role

Atribut warriorAtribut();
Atribut mageAtribut();
Atribut archerAtribut();

// Display
void karakter(Atribut player);
void displayPilihRole();
void displaySkill(adrNode root);

// Tree (CRUD)
void createTree(adrNode &root);
adrNode createNode(int idSkill, string namaSkill, int damage, int staminaCost, int mana);
void insertNode(adrNode &root, adrNode p);
void insertSkill(adrNode &root, int idSkill, string namaSkill, int damage, int staminaCost, int mana);
adrNode searchSkill(adrNode root, string namaSkill);

// Pemilihan Role

adrNode pilihRoleTree(int choice);
adrNode buildWarriorTree();
adrNode buildArcherTree();
adrNode buildMageTree();

// Pengecekan
void postOrder(adrNode root);

#endif // TURNBASE_H_INCLUDED
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

// Atribut Player Sesuai Role

Atribut warriorAtribut();
Atribut mageAtribut();
Atribut archerAtribut();

// Display
void karakter(Atribut player);
void displayPilihRole();
void displaySkill(adrNode root);

// Tree (CRUD)
void createTree(adrNode &root);
adrNode createNode(int idSkill, string namaSkill, int damage, int staminaCost, int mana);
void insertNode(adrNode &root, adrNode p);
void insertSkill(adrNode &root, int idSkill, string namaSkill, int damage, int staminaCost, int mana);
adrNode searchSkill(adrNode root, string namaSkill);

// Pemilihan Role

adrNode pilihRoleTree(int choice);
adrNode buildWarriorTree();
adrNode buildArcherTree();
adrNode buildMageTree();

// Pengecekan
void postOrder(adrNode root);

#endif // TURNBASE_H_INCLUDED

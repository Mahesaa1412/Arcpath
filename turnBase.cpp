#include <iostream>
#include "turnBase.h"
using namespace std;

void createTree(adrNode &root){
    root = nullptr;
}

adrNode createNode(string namaSkill, int damage, int staminaCost){
    adrNode p;
    p = new skillNode;
    p ->info.namaSkill = namaSkill;
    p ->info.damage = damage;
    p ->info.staminaCost = staminaCost;
    p ->left = nullptr;
    p ->right = nullptr;
    return p;
}

void insertNode(adrNode &root, adrNode p){
    if (root == nullptr){
        root = p;
    }else if (p->info.staminaCost < root->info.staminaCost){
        insertNode(root->left, p);
    }else if (p->info.staminaCost > root->info.staminaCost){
        insertNode(root->right, p);
    }
}

void insertSkill(adrNode &root, string namaSkill, int damage, int staminaCost){
    insertNode(root, createNode(namaSkill, damage, staminaCost));
}

adrNode buildWarriorTree(){
    adrNode root = nullptr;

    insertSkill(root, "Slash", 20, 10);
    insertSkill(root, "Iron_Skin", 0, 5);
    insertSkill(root, "Bash", 30, 15);
    insertSkill(root, "Power_Strike", 40, 20);
    insertSkill(root, "Shield_Bash", 0, 8);
    insertSkill(root, "Whirlwind", 50, 25);
    insertSkill(root, "Earthquake", 60, 30);
    insertSkill(root, "Curse Strike", 10, 12);

    return root;
}

adrNode buildArcherTree(){
    adrNode root = nullptr;
    insertSkill(root, "Quick_Shot", 15, 5);
    insertSkill(root, "Piercing_Arrow", 25, 10);
    insertSkill(root, "Multi_Shot", 35, 15);
    insertSkill(root, "Eagle_Eye", 0, 8);
    insertSkill(root, "Rapid_Fire", 40, 20);
    insertSkill(root, "Poison_Arrow", 10, 12);
    insertSkill(root, "Rain_of_Arrows", 50, 25);
    insertSkill(root, "Snipe", 60, 30);
    return root;
}

adrNode buildMageTree(){
    adrNode root = nullptr;
    insertSkill(root, "Fireball", 25, 10);
    insertSkill(root, "Ice_Spike", 20, 8);
    insertSkill(root, "Lightning_Bolt", 35, 15);
    insertSkill(root, "Mana_Shield", 0, 5);
    insertSkill(root, "Meteor", 50, 25);
    insertSkill(root, "Frost_Nova", 15, 12);
    insertSkill(root, "Arcane_Blast", 40, 20);
    insertSkill(root, "Curse", 10, 18);
    return root;
}

void displaySkill(adrNode root){
    if(root != nullptr){
        displaySkill(root->left);
        cout << " - " << root->info.namaSkill << "\n";
        displaySkill(root->right);
    }
}

adrNode searchSkill(adrNode root, string namaSkill){
    if (root == nullptr) {
        return nullptr;
    }

    if (root->info.namaSkill == namaSkill) {
        return root;
    }

    adrNode leftSkill = searchSkill(root->left, namaSkill);
    if (leftSkill != nullptr) {
        return leftSkill;
    }

    return searchSkill(root->right, namaSkill);
}

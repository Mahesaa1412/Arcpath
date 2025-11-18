#include "turnBase.h"

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


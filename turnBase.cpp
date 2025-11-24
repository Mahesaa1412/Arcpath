#include <iostream>
#include "turnBase.h"
using namespace std;

void displayPilihRole(){
    cout << "===================================\n";
    cout << "   <<  -- PILIH ROLE ANDA -- >>\n";
    cout << "===================================\n";
    cout << endl;
    cout << "----------------(1)---------------\n";
    cout << "              WARRIOR\n";
    cout << endl;
    cout << "       Bersifat kuat, tebal,\n";
    cout << "    dan ahli dalam pertarungan\n";
    cout << "            jarak dekat" << "\n\n";

    cout << "----------------(2)---------------\n";
    cout << "              ARCHER\n";
    cout << endl;
    cout << "    Cepat, presisi tinggi, dan\n";
    cout << " mematikan dalam serangan jarak jauh" << "\n\n";

    cout << "----------------(3)---------------\n";
    cout << "               MAGE\n";
    cout << endl;
    cout << "    Menguasai sihir elemen dan\n";
    cout << "    menghancurkan musuh dengan\n";
    cout << "       kekuatan mistis besar" << "\n\n";

    cout << "===================================\n";
    cout << ">> PILIH ROLE ANDA (1-3): ";
}

void createTree(adrNode &root){
    root = nullptr;
}

adrNode createNode(int idSkill, string namaSkill, int damage, int staminaCost, int mana){
    adrNode p;
    p = new skillNode;
    p ->info.idSkill = idSkill;
    p ->info.namaSkill = namaSkill;
    p ->info.damage = damage;
    p ->info.staminaCost = staminaCost;
    p ->info.mana = mana;
    p ->left = nullptr;
    p ->right = nullptr;
    return p;
}

void insertNode(adrNode &root, adrNode p){
    if (root == nullptr){
        root = p;
    }else if (p->info.idSkill < root->info.idSkill){
        insertNode(root->left, p);
    }else if (p->info.idSkill > root->info.idSkill){
        insertNode(root->right, p);
    }
}

void insertSkill(adrNode &root, int idSkill, string namaSkill, int damage, int staminaCost, int mana){
    insertNode(root, createNode(idSkill, namaSkill, damage, staminaCost, mana));
}

Atribut warriorAtribut(){
    Atribut a;
    a.maxHp = 150;
    a.maxStamina = 100;
    a.maxMana = 25;

    return a;
}

Atribut mageAtribut(){
    Atribut a;
    a.maxHp = 100;
    a.maxStamina = 25;
    a.maxMana = 150;

    return a;
}

Atribut archerAtribut(){
    Atribut a;
    a.maxHp = 100;
    a.maxStamina = 150;
    a.maxMana = 25;

    return a;
}

void karakter(Atribut player){
    cout << "maxHp: " << player.maxHp << endl;
    cout << "maxStamina: " << player.maxStamina << endl;
    cout << "maxMana: " << player.maxMana << endl;
}

adrNode pilihRoleTree(int choice){
    Atribut status;
    switch(choice){
        case 1:
            cout << "Kamu menjadi Warrior!"<< endl;
            status = warriorAtribut();
            karakter(status);
            cout << "\n=== Warrior Skill Tree ===\n";
            return buildWarriorTree();
        case 2:
            cout << "Kamu menjadi Archer!"<< endl;
            status = archerAtribut();
            karakter(status);
            cout << "\n=== Archer Skill Tree ===\n";
            return buildArcherTree();
        case 3:
            cout << "Kamu menjadi Mage!"<< endl;
            status = mageAtribut();
            karakter(status);
            cout << "\n=== Mage Skill Tree ===\n";
            return buildMageTree();
        default:
            cout << "Pilihan tidak valid!\n";
            return nullptr;
    }
}

adrNode buildWarriorTree(){
    adrNode root = nullptr;

    insertSkill(root, 9, "Slash", 20, 10, 0);
    insertSkill(root, 7, "Iron_Skin", 0, 5, 0);
    insertSkill(root, 3, "Bash", 30, 15, 0);
    insertSkill(root, 2, "Power_Strike", 40, 20, 0);
    insertSkill(root, 5, "Shield_Bash", 0, 8, 0);
    insertSkill(root, 6, "Whirlwind", 50, 25, 0);
    insertSkill(root, 4, "Earthquake", 60, 30, 0);
    insertSkill(root, 10, "Curse Strike", 10, 12, 0);

    return root;
}

adrNode buildArcherTree(){
    adrNode root = nullptr;
    insertSkill(root, 7, "Quick_Shot", 15, 5, 0);
    insertSkill(root, 2, "Piercing_Arrow", 25, 10, 0);
    insertSkill(root, 10, "Multi_Shot", 35, 15, 0);
    insertSkill(root, 4, "Eagle_Eye", 0, 8, 0);
    insertSkill(root, 6, "Rapid_Fire", 40, 20, 0);
    insertSkill(root, 14, "Poison_Arrow", 10, 12, 0);
    insertSkill(root, 20, "Rain_of_Arrows", 50, 25, 0);
    insertSkill(root, 8, "Snipe", 60, 30, 0);
    return root;
}

adrNode buildMageTree(){
    adrNode root = nullptr;
    insertSkill(root, 8, "Fireball", 25, 10, 100);
    insertSkill(root, 2, "Ice_Spike", 20, 8, 100);
    insertSkill(root, 1, "Lightning_Bolt", 35, 15, 100);
    insertSkill(root, 12, "Mana_Shield", 0, 5, 100);
    insertSkill(root, 5, "Meteor", 50, 25, 100);
    insertSkill(root, 7, "Frost_Nova", 15, 12, 100);
    insertSkill(root, 6, "Arcane_Blast", 40, 20, 100);
    insertSkill(root, 13, "Curse", 10, 18, 100);
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

void postOrder(adrNode root){
    if (root != nullptr){
        postOrder(root->left);
        postOrder(root->right);
        cout << root->info.idSkill;
        cout << root->info.namaSkill;
        cout << " | ";
    }
}

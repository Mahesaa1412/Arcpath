// Arcpath Turn-Based Combat dengan Skill dan Aksi
#include <iostream>
#include "turnBase.h"
using namespace std;

int main()
{
    adrNode selectedTree, root;
    int choice;
    selectedTree = nullptr;

    createTree(root);

// USER PILIH ROLE
    displayPilihRole(); // tampilan pilih role
    cin >> choice; //insert pilihan ada 1 2 3
    cout << endl;

    selectedTree = pilihRoleTree(choice); // switch case antara role
    if (selectedTree == nullptr){
        return 0;
    }

// MASUK KE LIST SKILL & USER PILIH SKILL
    cout << "\nSkill List:\n";
    displaySkill(selectedTree);

// CEK TREE
    cout << "POST-ORDER: ";
    postOrder(selectedTree);
// END CHECK

    string userPilihNamaSkill;
    adrNode ketemu;

    cout << endl;

    cout << "Pilih Skill: ";
    cin >> userPilihNamaSkill;
    ketemu = searchSkill(selectedTree, userPilihNamaSkill);

    if(ketemu != nullptr){
        cout << "\nSkill ditemukan!\n";
        cout << "Nama: " << ketemu->info.namaSkill << endl;
        cout << "Damage: " << ketemu->info.damage << endl;
        cout << "Stamina: " << ketemu->info.staminaCost << endl;
        cout << "Mana: " << ketemu->info.mana << endl;
    } else {
        cout << "Skill tidak ditemukan!\n";
    }

// USER PILIH ANTARA INGIN MENGGUNAKAN SKILL ATAU TIDAK

    cout << "Gunakan skill?\n";
    cout << "[IYA] atau [TIDAK]";

// ------------------ [END] UNTUK PILIH ROLE ------------------//
    return 0;
}

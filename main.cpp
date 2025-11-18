// Duely Tactics Turn-Based Combat dengan Skill dan Aksi
#include <iostream>
#include "turnBase.h"
using namespace std;

int main()
{
    adrNode selectedTree, root;
    int choice;
    selectedTree = nullptr;

    createTree(root);

//USER PILIH ROLE
    cout << "Pilih Role:\n";
    cout << "1. Warrior\n2. Archer\n3. Mage\n";
    cout << "Masukkan pilihan (1-3): ";
    cin >> choice;

    selectedTree = pilihRoleTree(choice);
    if (selectedTree == nullptr){
        return 0;
    }

//MASUK KE LIST SKILL
//USER PILIH SKILL
    cout << "\nSkill List:\n";
    displaySkill(selectedTree);

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
    } else {
        cout << "Skill tidak ditemukan!\n";
    }

//USER PILIH ANTARA INGIN MENGGUNAKAN SKILL ATAU TIDAK

    return 0;
}

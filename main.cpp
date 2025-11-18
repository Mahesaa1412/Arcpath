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

    cout << "Pilih Role:\n";
    cout << "1. Warrior\n2. Archer\n3. Mage\n";
    cout << "Masukkan pilihan (1-3): ";
    cin >> choice;

    switch(choice){
        case 1:
            cout << "\n=== Warrior Skill Tree ===\n";
            selectedTree = buildWarriorTree();
            break;
        case 2:
            cout << "\n=== Archer Skill Tree ===\n";
            selectedTree = buildArcherTree();
            break;
        case 3:
            cout << "\n=== Mage Skill Tree ===\n";
            selectedTree = buildMageTree();
            break;
        default:
            cout << "Pilihan tidak valid!\n";
            return 0;
    }

    cout << "\nSkill List:\n";
    displaySkill(selectedTree);

    return 0;
}


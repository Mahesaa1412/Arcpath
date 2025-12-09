#include <iostream>
#include <string>
#include "mll.h"
using namespace std;

int main() {
    Role* roleHead = NULL;
    Stage* stageHead = NULL;

    initRoles(roleHead);
    initStages(stageHead);

    int menu;

    while (true) {
        cout << "\n===== MENU UTAMA =====\n";
        cout << "1. Login Admin\n";
        cout << "2. Main Game\n";
        cout << "3. Exit\n";
        cout << ">> ";
        cin >> menu;

        if (menu == 1) {
            if (loginAdmin()) {
                int ch = 0;
                while (ch != 5) {
                    cout << "\n===== MENU ADMIN =====\n";
                    cout << "1. Lihat Skill\n";
                    cout << "2. Tambah Skill\n";
                    cout << "3. Update Skill\n";
                    cout << "4. Hapus Skill\n";
                    cout << "5. Logout\n";
                    cout << ">> ";
                    cin >> ch;

                    if (ch == 1) showSkills(roleHead);
                    else if (ch == 2) addSkill(roleHead);
                    else if (ch == 3) updateSkill(roleHead);
                    else if (ch == 4) deleteSkill(roleHead);
                }
            }
        }

        else if (menu == 2) {
            playGame(roleHead, stageHead);
        }

        else if (menu == 3) {
            cout << "Keluar...\n";
            break;
        }
    }

    return 0;
}

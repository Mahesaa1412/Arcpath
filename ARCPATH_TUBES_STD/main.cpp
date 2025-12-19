#include <iostream>
#include <string>
#include "mll.h"
using namespace std;

/* ---------------------------
   Main
   --------------------------- */
int main() {
    // build roles (parent list) with initial skills
    RoleNode* roles = createRoleList();
    cout << RED << "-----------------------------" << RESET << endl;
    cout << BLUE << "||" << RESET << BLINK << BOLD << YELLOW_CREAM << "     " << BG_BRED <<" A R C P A T H " << RESET << "     " << RESET << BLUE << "||" << RESET << endl;
    cout << RED << "-----------------------------" << RESET << endl;
    menu(roles);
    // cleanup
    freeRoleList(roles);

    cout << GOLD << ITALIC << "Goodbye!\n" << RESET;
    return 0;
}

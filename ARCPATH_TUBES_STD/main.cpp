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
    cout << RED << "||" << RESET << BLINK << BOLD << BYELLOW << "     " << BG_BRED <<" A R C P A T H " << RESET << "     " << RESET << RED << "||" << RESET << endl;
    cout << RED << "-----------------------------" << RESET << endl;
    menu(roles);
    // cleanup
    freeRoleList(roles);

    cout << "Goodbye!\n";
    return 0;
}

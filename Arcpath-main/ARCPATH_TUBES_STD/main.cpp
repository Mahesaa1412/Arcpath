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
    menu(roles);
    // cleanup
    freeRoleList(roles);

    cout << "Goodbye!\n";
    return 0;
}

#include <iostream>
#include <string>
#include "mll.h"
using namespace std;

/* ---------------------------
   Helper functions for gameplay
   --------------------------- */

// find first alive enemy in enemy list
EnemyNode* firstAliveEnemy(EnemyNode* head) {
    EnemyNode* p = head;
    while (p != nullptr) {
        if (p->hp > 0) return p;
        p = p->next;
    }
    return nullptr;
}

// check if all enemies dead
bool allEnemiesDead(EnemyNode* head) {
    EnemyNode* p = head;
    while (p != nullptr) {
        if (p->hp > 0) return false;
        p = p->next;
    }
    return true;
}

// apply damage to a single enemy node
void damageEnemy(EnemyNode* e, int dmg) {
    if (e == nullptr) return;
    e->hp -= dmg;
    if (e->hp < 0) e->hp = 0;
}

// enemies attack player; defense reduces flat damage
void enemiesAttack(EnemyNode* head, int &playerHp, int defense) {
    EnemyNode* p = head;
    while (p != nullptr) {
        if (p->hp > 0) {
            int incoming = p->atk - defense;
            if (incoming < 0) incoming = 0;
            playerHp -= incoming;
            cout << p->name << " hits you for " << incoming << " dmg.\n";
            if (playerHp <= 0) { playerHp = 0; return; }
        }
        p = p->next;
    }
}

// print player and enemies
void printBattleStatus(int hp, int maxHp, int stamina, int maxStamina, int defense, StageNode* stage) {
    cout << "\n-- Player --\nHP: " << hp << "/" << maxHp << " | Stamina: " << stamina << "/" << maxStamina;
    if (defense > 0) cout << " | Defense: " << defense;
    cout << "\n";
    printStageEnemies(stage);
}

// copy skills from RoleNode into an indexed array for convenient choice
int copySkillsToArray(RoleNode* role, SkillNode* arr[], int maxArr) {
    int i = 0;
    SkillNode* p = role->skillHead;
    while (p != nullptr && i < maxArr) {
        arr[i] = p;
        i++;
        p = p->next;
    }
    return i;
}

/* ---------------------------
   The battle loop per playthrough
   --------------------------- */
void playGame(RoleNode* rolesHead) {
    // build fresh stages (fresh enemies)
    StageNode* stages = buildDefaultStages();

    // choose role
    printAllRoles(rolesHead);
    cout << "Choose role (1-3): ";
    int rIdx; cin >> rIdx;
    RoleNode* chosen = findRoleByIndex(rolesHead, rIdx);
    if (chosen == nullptr) { cout << "Invalid role.\n"; freeStages(stages); return; }

    // player stats by role
    int maxHp = 100, maxStamina = 60;
    if (chosen->roleName == "Warrior") { maxHp = 160; maxStamina = 60; }
    else if (chosen->roleName == "Archer") { maxHp = 120; maxStamina = 80; }
    else if (chosen->roleName == "Mage") { maxHp = 100; maxStamina = 100; }

    int hp = maxHp;
    int stamina = maxStamina;
    int defense = 0;
    int buffTurns = 0;

    // copy skills into array
    const int MAX_SK = 8;
    SkillNode* skArr[MAX_SK];
    int skCount = copySkillsToArray(chosen, skArr, MAX_SK);

    if (skCount == 0) {
        cout << "This role has no skills. Ask admin to add skills first.\n";
        freeStages(stages);
        return;
    }

    // iterate stages 1..10
    for (int s = 1; s <= 10; ++s) {
        StageNode* curStage = getStageByNumber(stages, s);
        cout << "\n========== STAGE " << s;
        if (s == 5 || s == 10) cout << " (BOSS)";
        cout << " ==========\n";

        // battle loop
        int turn = 1;
        while (!allEnemiesDead(curStage->enemyHead) && hp > 0) {
            cout << "\n-- Turn " << turn << " --\n";
            printBattleStatus(hp, maxHp, stamina, maxStamina, defense, curStage);

            cout << "\nActions:\n1. Basic Attack (cost 1 STA, dmg 10 to first alive enemy)\n2. Use Skill\n3. Rest (HP +5, Stamina +8)\nChoose: ";
            int act; cin >> act;

            if (act == 1) {
                if (stamina < 1) { cout << "Not enough stamina.\n"; }
                else {
                    stamina -= 1;
                    EnemyNode* target = firstAliveEnemy(curStage->enemyHead);
                    if (target != nullptr) {
                        cout << "You basic-attack " << target->name << " for 10 dmg.\n";
                        damageEnemy(target, 10);
                        if (target->hp == 0) cout << target->name << " defeated!\n";
                    }
                }
            }
            else if (act == 2) {
                // list skills
                skCount = copySkillsToArray(chosen, skArr, MAX_SK);
                cout << "Skills:\n";
                for (int i = 0; i < skCount; ++i) {
                    cout << (i+1) << ". " << skArr[i]->name
                         << " | type=" << skArr[i]->type
                         << " | val=" << skArr[i]->value
                         << " | STA=" << skArr[i]->staminaCost << "\n";
                }
                cout << "0. Cancel\nChoose skill: ";
                int si; cin >> si;
                if (si <= 0 || si > skCount) { cout << "Cancelled.\n"; }
                else {
                    SkillNode* sk = skArr[si-1];
                    if (stamina < sk->staminaCost) {
                        cout << "Not enough stamina.\n";
                    } else {
                        stamina -= sk->staminaCost;
                        if (sk->type == 0) {
                            // single target damage
                            EnemyNode* target = firstAliveEnemy(curStage->enemyHead);
                            if (target != nullptr) {
                                cout << "You use " << sk->name << " dealing " << sk->value << " dmg to " << target->name << ".\n";
                                damageEnemy(target, sk->value);
                                if (target->hp == 0) cout << target->name << " defeated!\n";
                            }
                        } else if (sk->type == 1) {
                            // buff defense: use value as flat defense and lasts 3 turns
                            defense += sk->value;
                            buffTurns = 3;
                            cout << "You use " << sk->name << ". Defense +" << sk->value << " for 3 turns.\n";
                        } else if (sk->type == 2) {
                            // AOE: damage all enemies
                            cout << "You use " << sk->name << " (AOE) dealing " << sk->value << " to all enemies.\n";
                            EnemyNode* e = curStage->enemyHead;
                            while (e != nullptr) {
                                if (e->hp > 0) {
                                    damageEnemy(e, sk->value);
                                    if (e->hp == 0) cout << e->name << " defeated!\n";
                                }
                                e = e->next;
                            }
                        } else if (sk->type == 3) {
                            // heal
                            hp += sk->value;
                            if (hp > maxHp) hp = maxHp;
                            cout << "You use " << sk->name << " and heal " << sk->value << " HP.\n";
                        } else {
                            cout << "Unknown skill type.\n";
                        }
                    }
                }
            }
            else if (act == 3) {
                // Rest
                int heal = 5;
                int restSta = 8;
                hp += heal; if (hp > maxHp) hp = maxHp;
                stamina += restSta; if (stamina > maxStamina) stamina = maxStamina;
                cout << "You rest: HP +" << heal << ", Stamina +" << restSta << ".\n";
            }
            else {
                cout << "Invalid action.\n";
            }

            // if all enemies died after player action => skip enemy turn
            if (allEnemiesDead(curStage->enemyHead)) break;

            // enemies' turn
            cout << "\n-- Enemies' Turn --\n";
            enemiesAttack(curStage->enemyHead, hp, defense);
            if (hp <= 0) { cout << "You have been defeated.\n"; break; }

            // reduce buff turns
            if (buffTurns > 0) {
                buffTurns--;
                if (buffTurns == 0) {
                    cout << "Your defense buff expired.\n";
                    defense = 0;
                }
            }

            turn++;
        } // end battle loop for stage

        if (hp <= 0) {
            cout << "\nGAME OVER at Stage " << s << ".\n";
            freeStages(stages);
            return;
        }

        // Stage cleared: reward regen
        int hpGain = 10 + s;
        int staGain = 5 + (s / 2);
        hp += hpGain; if (hp > maxHp) hp = maxHp;
        stamina += staGain; if (stamina > maxStamina) stamina = maxStamina;
        cout << "\nSTAGE " << s << " CLEARED! HP +" << hpGain << ", Stamina +" << staGain << ".\n";
    } // end stages

    cout << "\nCONGRATULATIONS! You cleared all stages!\n";
    freeStages(stages);
}

/* ---------------------------
   Admin menu for CRUD operations
   --------------------------- */
void adminMenu(RoleNode* roles) {
    while (true) {
        cout << "\n-- ADMIN MENU --\n";
        cout << "1. View skills of a role\n";
        cout << "2. Add skill to a role\n";
        cout << "3. Update skill of a role\n";
        cout << "4. Delete skill of a role\n";
        cout << "5. Back\nChoose: ";
        int ch; cin >> ch;
        if (ch == 6) break;

        if (ch == 1) {
            printAllRoles(roles);
            cout << "Choose role index: "; int idx; cin >> idx;
            RoleNode* r = findRoleByIndex(roles, idx);
            if (r) printSkillsOfRole(r);
        } else if (ch == 2) {
            printAllRoles(roles);
            cout << "Choose role index to add skill: "; int idx; cin >> idx;
            RoleNode* r = findRoleByIndex(roles, idx);
            if (r) {
                int curr = countSkills(r);
                if (curr >= 4) { cout << "Role already has 4 skills.\n"; continue; }
                string nm; int val, st, tp;
                cout << "Skill name: "; cin >> nm;
                cout << "Value (dmg/buff/heal): "; cin >> val;
                cout << "Stamina cost: "; cin >> st;
                cout << "Type (0=dmg,1=buff,2=aoe,3=heal): "; cin >> tp;
                addSkillToRole(r, nm, val, st, tp);
                cout << "Skill added.\n";
            }
        } else if (ch == 3) {
            printAllRoles(roles);
            cout << "Choose role index to update skill: "; int ridx; cin >> ridx;
            RoleNode* r = findRoleByIndex(roles, ridx);
            if (r) {
                printSkillsOfRole(r);
                cout << "Choose skill index to update: "; int sidx; cin >> sidx;
                string nm; int val, st, tp;
                cout << "New name: "; cin >> nm;
                cout << "New value: "; cin >> val;
                cout << "New stamina cost: "; cin >> st;
                cout << "New type: "; cin >> tp;
                updateSkillInRole(r, sidx, nm, val, st, tp);
                cout << "Skill updated.\n";
            }
        } else if (ch == 4) {
            printAllRoles(roles);
            cout << "Choose role index to delete skill from: "; int ridx; cin >> ridx;
            RoleNode* r = findRoleByIndex(roles, ridx);
            if (r) {
                printSkillsOfRole(r);
                cout << "Choose skill index to delete: "; int sidx; cin >> sidx;
                removeSkillFromRole(r, sidx);
                cout << "Skill deleted (if index valid).\n";
            }
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

/* ---------------------------
   Main
   --------------------------- */
int main() {
    // build roles (parent list) with initial skills
    RoleNode* roles = createRoleList();

    while (true) {
        cout << "\n=== MAIN MENU ===\n1. Admin\n2. Player\n3. Exit\nChoose: ";
        int c;
        cin >> c;
        switch (c) {
            case 1: {
                if (loginAdmin()){
                    adminMenu(roles);
                } else {
                    break;
                }
            }
            case 2: {
                playGame(roles);
            }
            case 3: {
                break;
            }
            default: {
                cout << "Invalid.\n";
            }
        }
    }

    // cleanup
    freeRoleList(roles);

    cout << "Goodbye!\n";
    return 0;
}

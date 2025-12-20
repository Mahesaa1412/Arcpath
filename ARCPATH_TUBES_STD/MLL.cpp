#include "mll.h"

// MAIN MENU
void menu(RoleNode* roles){
    while (true) {
        cout << BYELLOW << "=====" << BRED << "[ " << BYELLOW <<  "   MAIN MENU   " << BRED <<  " ]" << BYELLOW << "=====\n" << RESET;
        cout << "1. Admin menu\n";
        cout << "2. Play game\n";
        cout << "3. Exit\n";
        cout << BRED << "-----------------------------\n" << RESET;
        cout << "Choose: ";

        int c;
        cin >> c;
        cout << endl;
        switch (c) {
            case 1: {
                if (loginAdmin()){
                    adminMenu(roles);
                }
                break;
            }
            case 2: {
                playGame(roles);
                break;
            }
            case 3: {
                return;
                break;
            }
            default: {
                cout << "Invalid.\n";
                break;
            }
        }
    }
}

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
    cout << YELLOW_CREAM << " ---------------------------\n";
    cout << "|" << RESET <<"          PLAYER           " << YELLOW_CREAM << "|\n";
    cout << " ---------------------------\n" << RESET;
    cout << "      HP : " << hp << " / " << maxHp << endl;
    cout << "      Stamina: " << stamina << " / " << maxStamina << endl;
    if (defense > 0) {
        cout << "      Defense: " << defense << endl;
    }
    cout << endl;
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
    int rIdx;
    cin >> rIdx;
    RoleNode* chosen = findRoleByIndex(rolesHead, rIdx);

    if (chosen == nullptr) {
        cout << "Invalid role.\n";
        freeStages(stages);
        return;
    }

    // player stats by role
    int maxHp = 100, maxStamina = 60;

    if (chosen->roleName == "Warrior") {
        maxHp = 200;
        maxStamina = 60;
    }else if (chosen->roleName == "Archer") {
        maxHp = 150;
        maxStamina = 100;
    }else if (chosen->roleName == "Mage") {
        maxHp = 120;
        maxStamina = 150;
    }

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
        cout << BLINK << BRED <<"\n======<" << RESET << BOLD << "    STAGE " << s << RESET;
        if (s == 5 || s == 10) {
            cout << " (BOSS)" << RESET;
        }
        cout << BLINK << BRED << "    >======\n" << RESET;

        // battle loop
        int turn = 1;
        while (!allEnemiesDead(curStage->enemyHead) && hp > 0) {
            bool playerActed = false; //cek apakah player bertindak

            cout << ITALIC << "\n         { Turn " << turn << " }\n" << RESET;
            printBattleStatus(hp, maxHp, stamina, maxStamina, defense, curStage);

            cout << YELLOW_CREAM << " ---------------------------" << endl;
            cout << "|" << RESET << "          ACTIONS          " << YELLOW_CREAM << "|" << endl;
            cout << " ---------------------------" << RESET << endl;
            cout << "1. Basic Attack (10 damage)" << endl;
            cout << "2. Use Skill" << endl;
            cout << "3. Rest (HP +10, Stamina +8)" << endl;
            cout << YELLOW_CREAM << " ---------------------------" << RESET << endl;
            cout << "Choose: ";
            int act;
            cin >> act;
            cout << endl;

            if (act == 1) {
                if (stamina < 1) {
                    cout << "Not enough stamina.\n";
                } else {
                    playerActed = true;
                    EnemyNode* target = firstAliveEnemy(curStage->enemyHead);
                    if (target != nullptr) {
                        cout << "You use basic-attack on " << target->name << " for 10 dmg.\n";
                        damageEnemy(target, 10);
                        this_thread::sleep_for(chrono::milliseconds(1500));
                        if (target->hp == 0) {
                            cout << target->name << " defeated!\n";
                            this_thread::sleep_for(chrono::milliseconds(1500));
                        }
                    }
                }
            } else if (act == 2) {
                // list skills
                skCount = copySkillsToArray(chosen, skArr, MAX_SK);
                cout << "Skills:\n";
                for (int i = 0; i < skCount; ++i) {
                        if (skArr[i]->type == "damage") {
                            cout << (i+1) << ". " << BRED << skArr[i]->name << RESET
                                 << " (" << skArr[i]->value << "/" << skArr[i]->staminaCost << ")\n";
                        } else if (skArr[i]->type == "defense"){
                            cout << (i+1) << ". " << BYELLOW << skArr[i]->name << RESET
                                 << " (" << skArr[i]->value << "/" << skArr[i]->staminaCost << ")\n";
                        } else if (skArr[i]->type == "heal"){
                            cout << (i+1) << ". " << BGREEN << skArr[i]->name << RESET
                                 << " (" << skArr[i]->value << "/" << skArr[i]->staminaCost << ")\n";
                        } else if (skArr[i]->type == "AOE"){
                            cout << (i+1) << ". " << BORANGE << skArr[i]->name << RESET
                                 << " (" << skArr[i]->value << "/" << skArr[i]->staminaCost << ")\n";
                        }
                }
                cout << "0. Cancel\nChoose skill: ";
                int si;
                cin >> si;
                if (si <= 0 || si > skCount) {
                    cout << "Cancelled.\n";
                } else {
                    SkillNode* sk = skArr[si-1];
                    if (stamina < sk->staminaCost) {
                        cout << "Not enough stamina.\n";
                    } else {
                        playerActed = true;
                        stamina -= sk->staminaCost;
                        if (sk->type == "damage") {
                            // single target damage
                            EnemyNode* target = firstAliveEnemy(curStage->enemyHead);
                            if (target != nullptr) {
                                cout << "You use " << sk->name << " dealing " << sk->value << " dmg to " << target->name << ".\n";
                                damageEnemy(target, sk->value);
                                this_thread::sleep_for(chrono::milliseconds(1500));
                                if (target->hp == 0) {
                                    cout << target->name << " defeated!\n";
                                    this_thread::sleep_for(chrono::milliseconds(1500));
                                }
                            }
                        } else if (sk->type == "defense") {
                            // buff defense: use value as flat defense and lasts 2 turns
                            defense += sk->value;
                            buffTurns = 2;
                            cout << "You use " << sk->name << ". Defense +" << sk->value << " for 1 turns.\n";
                        } else if (sk->type == "AOE") {
                            // AOE: damage all enemies
                            cout << "You use " << sk->name << " (AOE) dealing " << sk->value << " to all enemies.\n";
                            this_thread::sleep_for(chrono::milliseconds(1500));
                            EnemyNode* e = curStage->enemyHead;
                            while (e != nullptr) {
                                if (e->hp > 0) {
                                    damageEnemy(e, sk->value);
                                    if (e->hp == 0) {
                                            cout << e->name << " defeated!\n";
                                            this_thread::sleep_for(chrono::milliseconds(1500));
                                    }
                                }
                                e = e->next;
                            }
                        } else if (sk->type == "heal") {
                            // heal
                            hp += sk->value;
                            if (hp > maxHp) hp = maxHp;
                            cout << "You use " << sk->name << " and heal " << sk->value << " HP.\n";
                            this_thread::sleep_for(chrono::milliseconds(1500));
                        } else {
                            cout << "Unknown skill type.\n";
                        }
                    }
                }
            }
            else if (act == 3) {
                playerActed = true;
                // Rest
                int heal = 10;
                int restSta = 8;
                hp += heal;
                if (hp > maxHp) {

                    hp = maxHp;

                }
                stamina += restSta;
                if (stamina > maxStamina) {
                    stamina = maxStamina;
                    cout << "You rest: HP +" << heal << ", Stamina +" << restSta << ".\n";
                    this_thread::sleep_for(chrono::milliseconds(1500));
                }
            } else if (act > 3) {
                cout << "Invalid action.\n";
            }

            if (playerActed){
                // if all enemies died after player action => skip enemy turn
                if (allEnemiesDead(curStage->enemyHead)) {
                    break;
                }

                // enemies' turn
                cout << "\n-- Enemies' Turn --\n";
                enemiesAttack(curStage->enemyHead, hp, defense);
                if (hp <= 0) {
                    cout << "You have been defeated.\n";
                    break;
                }

                // reduce buff turns
                if (buffTurns > 0) {
                    buffTurns--;
                    if (buffTurns == 0) {
                        cout << "Your defense buff expired.\n";
                        defense = 0;
                    }
                }
                turn++;
            }
        } // end battle loop for stage

        if (hp <= 0) {
            cout << BOLD << BLINK << BRED << "\nGAME OVER at Stage (" << s << ")\n" << RESET;
            cout << endl;
            freeStages(stages);
            return;
        }

        // Stage cleared: reward regen
        int hpGain = 30 + s;
        int staGain = 15 + (s / 2);
        hp += hpGain; if (hp > maxHp) hp = maxHp;
        stamina += staGain;
        if (stamina > maxStamina) {
            stamina = maxStamina;
        }
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
        cout << BYELLOW << "\n======[  ADMIN  MENU  ]======\n" << RESET;
        cout << "1. View skills of a role\n";
        cout << "2. Add skill to a role\n";
        cout << "3. Update skill of a role\n";
        cout << "4. Delete skill of a role\n";
        cout << "5. Back\n";
        cout << BYELLOW << "-----------------------------\n" << RESET;
        cout << "Choose: ";

        int ch;
        cin >> ch;
        cout << endl;

        switch (ch) {
            case 1: {
                printAllRoles(roles);
                cout << "Choose a role: ";
                int idx;
                cin >> idx;
                cout << endl;
                RoleNode* r = findRoleByIndex(roles, idx);
                if (r) {
                    printSkillsOfRole(r);
                }
                break;
            }
            case 2: {
                printAllRoles(roles);
                cout << "Choose role index to add skill: ";
                int idx;
                cin >> idx;
                RoleNode* r = findRoleByIndex(roles, idx);

                if (r) {
                    int curr = countSkills(r);
                    if (curr >= 5) {
                        cout << "Role already has 5 skills.\n";
                        break;
                    }

                    string nm, tp;
                    int val, st;
                    cout << "Skill name : ";
                    cin >> nm;
                    cout << "Value : ";
                    cin >> val;
                    cout << "Stamina cost : ";
                    cin >> st;
                    cout << "Type [damage|defense|AOE|heal] : ";
                    cin >> tp;
                    addSkillToRole(r, nm, val, st, tp);
                    cout << endl;
                    cout << BGREEN << "Skill added!\n" << RESET;
                }
                break;
            }
            case 3: {
                printAllRoles(roles);
                cout << "Choose role index to update skill: ";
                int ridx;
                cin >> ridx;
                RoleNode* r = findRoleByIndex(roles, ridx);
                if (r) {
                    printSkillsOfRole(r);
                    cout << "Choose skill index to update: ";
                    int sidx;
                    cin >> sidx;
                    if (sidx > countSkills(roles)) {
                        cout << BRED << "Invalid input!\n" << RESET;
                    } else {
                        string nm, tp;
                        int val, st;
                        cout << "New name: ";
                        cin >> nm;  
                        cout << "New value: ";
                        cin >> val;
                        cout << "New stamina cost: ";
                        cin >> st;
                        cout << "New type: ";
                        cin >> tp;
                        updateSkillInRole(r, sidx, nm, val, st, tp);
                        cout << endl;
                        cout << BGREEN << "Skill updated!\n" << RESET;
                    }
                }
                break;
            }
            case 4: {
                printAllRoles(roles);
                cout << "Choose role index to delete skill from: ";
                int ridx;
                cin >> ridx;
                RoleNode* r = findRoleByIndex(roles, ridx);
                if (r) {
                    printSkillsOfRole(r);
                    cout << "Choose skill index to delete: ";
                    int sidx;
                    cin >> sidx;
                    if (sidx > countSkills(roles)) {
                        cout << BRED << "Invalid input!\n" << RESET;
                    } else {
                        removeSkillFromRole(r, sidx);
                        cout << BGREEN << "Skill deleted!\n" << RESET;
                    }
                }
                break;
            }
            case 5: {
                return;
                break;
            }
            default: {
                cout << "Invalid choice.\n";
                break;
            }
        }
    }
}
/* ===========================
   ROLE & SKILL (MLL) METHODS
   =========================== */

// create Role list with three roles and some default skills
RoleNode* createRoleList() {
    // create three parent nodes: Warrior, Archer, Mage
    RoleNode* head = new RoleNode;
    head->roleName = "Warrior";
    head->skillHead = nullptr;
    head->next = new RoleNode;

    head->next->roleName = "Archer";
    head->next->skillHead = nullptr;
    head->next->next = new RoleNode;

    head->next->next->roleName = "Mage";
    head->next->next->skillHead = nullptr;
    head->next->next->next = nullptr;

    // add some default skills (so player can test immediately)
    addSkillToRole(head, "Slash", 40, 13, "damage");           // warrior
    addSkillToRole(head, "IronSkin", 10, 5, "defense");         // buff def
    addSkillToRole(head, "BladeStorm", 27, 10, "AOE"); // AOE
    addSkillToRole(head->next, "DoubleStrafe", 40, 10, "damage"); // archer
    addSkillToRole(head->next, "RainOfArrows", 25, 15, "AOE");// archer AOE
    addSkillToRole(head->next, "SharpShot", 90, 35, "damage");
    addSkillToRole(head->next->next, "Fireball", 25, 17, "AOE"); // mage
    addSkillToRole(head->next->next, "JupitelThunder", 50, 25, "damage");
    addSkillToRole(head->next->next, "HealingLight", 25, 5, "heal"); // mage heal

    return head;
}

void freeSkillList(SkillNode* head) {
    while (head != nullptr) {
        SkillNode* nx = head->next;
        delete head;
        head = nx;
    }
}

// free roles and associated skill child lists
void freeRoleList(RoleNode* head) {
    RoleNode* r = head;
    while (r != nullptr) {
        freeSkillList(r->skillHead);
        RoleNode* nx = r->next;
        delete r;
        r = nx;
    }
}

int countSkills(RoleNode* role) {
    int c = 0;
    SkillNode* p = role->skillHead;
    while (p != nullptr) {
        c++;
        p = p->next;
    }
    return c;
}

SkillNode* findSkillByIndex(RoleNode* role, int idx) {
    if (idx <= 0) return nullptr;
    SkillNode* p = role->skillHead;
    int i = 1;
    while (p != nullptr && i < idx) { p = p->next; i++; }
    return p;
}

// add skill child to role (append). Will not check max here (admin code should).
void addSkillToRole(RoleNode* role, const string &name, int value, int stamina, string type) {
    SkillNode* n = new SkillNode;
    n->name = name;
    n->value = value;
    n->staminaCost = stamina;
    n->type = type;
    n->next = nullptr;

    if (role->skillHead == nullptr) role->skillHead = n;
    else {
        SkillNode* p = role->skillHead;
        while (p->next != nullptr) p = p->next;
        p->next = n;
    }
}

// remove skill by 1-based index
void removeSkillFromRole(RoleNode* role, int index) {
    if (index <= 0) return;
    SkillNode* curr = role->skillHead;
    SkillNode* prev = nullptr;
    int i = 1;
    while (curr != nullptr && i < index) {
        prev = curr; curr = curr->next; i++;
    }
    if (curr == nullptr) return;
    if (prev == nullptr) role->skillHead = curr->next;
    else prev->next = curr->next;
    delete curr;
}

// update skill at index (1-based)
void updateSkillInRole(RoleNode* role, int index, const string &name, int value, int stamina, string type) {
    SkillNode* s = findSkillByIndex(role, index);
    if (s == nullptr) return;
    s->name = name;
    s->value = value;
    s->staminaCost = stamina;
    s->type = type;
}

void printSkillsOfRole(RoleNode* role) {
    if (role->roleName == "Warrior") {
        cout << "\n=+=  " << BG_BGRAY << BLACK << BLINK << " Skills of " << RED << role->roleName << " " << RESET << "  =+=\n";
    } else if (role->roleName == "Archer") {
        cout << "\n=+=  " << BG_BGRAY << BLACK << BLINK << " Skills of " << GREEN << role->roleName << " " << RESET << "  =+=\n";
    } else if (role->roleName == "Mage") {
        cout << "\n=+=  " << BG_BGRAY << BLACK << BLINK << " Skills of " << BLUE << role->roleName << " " << RESET << "  =+=\n";
    }

    SkillNode* p = role->skillHead;

    if (p == nullptr){
        cout << BRED << " ( ! No skills available ! )\n" << RESET;
    }

    int i = 1;
    while (p != nullptr) {
        cout << "=============================\n";
        cout << i << ". " << p->name << "\n";
        cout << "=============================\n";
        cout << "- Skill Type    : " << p->type << "\n";
        cout << "- Value         : " << p->value << "\n";
        cout << "- Stamina       : " << p->staminaCost << "\n";
        p = p->next;
        i++;
    }
    cout << endl;
}

RoleNode* findRoleByIndex(RoleNode* head, int idx) {
    if (idx <= 0) return nullptr;
    RoleNode* p = head;
    int i = 1;
    while (p != nullptr && i < idx) {
            p = p->next;
            i++;
    }
    return p;
}

void printAllRoles(RoleNode* head) {
    cout << BYELLOW << "\n======[     ROLES     ]======\n" << RESET ;
    RoleNode* p = head;
    int i = 1;
    while (p != nullptr) {
        cout << i << ". " << p->roleName << " (skills: " << countSkills(p) << ")\n";
        p = p->next;
        i++;
    }
    cout << BYELLOW << "-----------------------------\n" << RESET;
}

/* ===========================
   STAGE & ENEMY (MLL) METHODS
   =========================== */

// create an empty stage list with numbers; we will attach enemy child lists after that
StageNode* buildDefaultStages() {
    StageNode* head = nullptr;
    StageNode* tail = nullptr;

    for (int i = 1; i <= 10; ++i) {
        StageNode* s = new StageNode;
        s->stageNum = i;
        s->enemyHead = nullptr;
        s->next = nullptr;
        if (head == nullptr) head = s;
        else tail->next = s;
        tail = s;
    }

    // fill children (enemies)
    // Stage 1
    StageNode* st1 = head;
    addEnemyToStage(st1, "Goblin", 20, 8);

    // Stage 2
    addEnemyToStage(st1->next, "Wolf", 30, 8);
    addEnemyToStage(st1->next, "Wolf Pup", 15, 5);

    // Stage 3
    addEnemyToStage(st1->next->next, "Bandit", 35, 8);
    addEnemyToStage(st1->next->next, "Bandit Archer", 20, 12);

    // Stage 4
    addEnemyToStage(st1->next->next->next, "Skeleton", 13, 5);
    addEnemyToStage(st1->next->next->next, "Skeleton Archer", 13, 15);
    addEnemyToStage(st1->next->next->next, "Skeleton Knight", 40, 13);

    // Stage 5 - boss
    addEnemyToStage(st1->next->next->next->next, "Orc Chieftain (BOSS)", 150, 20);

    // Stage 6
    addEnemyToStage(st1->next->next->next->next->next, "Goblin Thief", 25, 9);
    addEnemyToStage(st1->next->next->next->next->next, "Goblin Shaman", 40, 13);

    // Stage 7
    addEnemyToStage(st1->next->next->next->next->next->next, "Dark Mage", 25, 22);
    addEnemyToStage(st1->next->next->next->next->next->next, "Elite Skeleton", 35, 7);

    // Stage 8
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band of Thieves A", 20, 12);
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band of Thieves B", 20, 12);
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band Leader", 50, 15);

    // Stage 9
    addEnemyToStage(st1->next->next->next->next->next->next->next->next, "Giant Spider", 100, 18);
    addEnemyToStage(st1->next->next->next->next->next->next->next->next, "Spider", 30, 10);

    // Stage 10 - boss multi
    addEnemyToStage(st1->next->next->next->next->next->next->next->next->next, "Ancient Dragon (BOSS)", 250, 23);

    return head;
}

void freeEnemyList(EnemyNode* head) {
    while (head != nullptr) {
        EnemyNode* nx = head->next;
        delete head;
        head = nx;
    }
}

void freeStages(StageNode* head) {
    StageNode* s = head;
    while (s != nullptr) {
        freeEnemyList(s->enemyHead);
        StageNode* nx = s->next;
        delete s;
        s = nx;
    }
}

StageNode* getStageByNumber(StageNode* head, int num) {
    StageNode* p = head;
    while (p != nullptr) {
        if (p->stageNum == num) return p;
        p = p->next;
    }
    return nullptr;
}

void addEnemyToStage(StageNode* stage, const string &name, int hp, int atk) {
    if (stage == nullptr) return;
    EnemyNode* n = new EnemyNode;
    n->name = name;
    n->hp = hp;
    n->atk = atk;
    n->next = nullptr;

    if (stage->enemyHead == nullptr) stage->enemyHead = n;
    else {
        EnemyNode* p = stage->enemyHead;
        while (p->next != nullptr) p = p->next;
        p->next = n;
    }
}

int countEnemies(StageNode* stage) {
    int c = 0;
    EnemyNode* p = stage->enemyHead;
    while (p != nullptr) { c++; p = p->next; }
    return c;
}

void printStageEnemies(StageNode* stage) {
    if (stage == nullptr) {
        return;
    }
    cout << YELLOW_CREAM << " ---------------------------\n";
    cout << "|" << RESET << "          ENEMIES          " << YELLOW_CREAM << "|\n";
    cout << " ---------------------------\n" << RESET;
    EnemyNode* p = stage->enemyHead;
    int i = 1;
    while (p != nullptr) {
        cout << i << ". " << p->name << endl;
        cout << "    HP  : " << p->hp << endl;
        cout << "    ATK : " << p->atk << endl;
        cout << endl;
        p = p->next;
        i++;
    }
}

bool loginAdmin() {
    string user, pass;

    cout << BYELLOW <<"\n======[  LOGIN ADMIN  ]======\n" << RESET;
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    cout << endl;

    if (user == "admin" && pass == "admin123") {
        cout << GREEN << "----" << BLINK << "!! " << RESET << "Login Successful" << GREEN << BLINK << " !!" << RESET << GREEN << "----" << RESET;
        cout << endl;
        return true;
    }

    cout << BLINK << BRED << "!" << RESET << "Username or password is wrong" << BLINK << BRED << "!\n" << RESET;
    cout << "\n";
    cout << endl;
    return false;
}





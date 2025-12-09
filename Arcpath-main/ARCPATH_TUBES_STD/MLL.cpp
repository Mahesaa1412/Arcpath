#include "mll.h"

// MAIN MENU
void menu(RoleNode* roles){
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
                return;
            }
            default: {
                cout << "Invalid.\n";
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
            int act;
            cin >> act;

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
            else if (act > 3) {
                cout << "Invalid action.\n";
            }

            if (act <= 3){
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
            }
        } // end battle loop for stage

        if (hp <= 0) {
            cout << "\nGAME OVER at Stage " << s << ".\n";
            freeStages(stages);
            menu(rolesHead);
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

        int ch;
        cin >> ch;
        switch (ch) {
            case 1: {
                printAllRoles(roles);
                cout << "Choose role index: "; int idx; cin >> idx;
                RoleNode* r = findRoleByIndex(roles, idx);
                if (r) {
                    printSkillsOfRole(r);
                }
                adminMenu(roles);
            }
            case 2: {
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
                    adminMenu(roles);
                }
            }
            case 3: {
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
                    adminMenu(roles);
                }
            }
            case 4: {
                printAllRoles(roles);
                cout << "Choose role index to delete skill from: "; int ridx; cin >> ridx;
                RoleNode* r = findRoleByIndex(roles, ridx);
                if (r) {
                    printSkillsOfRole(r);
                    cout << "Choose skill index to delete: "; int sidx; cin >> sidx;
                    removeSkillFromRole(r, sidx);
                    cout << "Skill deleted (if index valid).\n";
                    adminMenu(roles);
                }
            }
            case 5: {
                menu(roles);
            }
            default: {
                cout << "Invalid choice.\n";
                adminMenu(roles);
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
    addSkillToRole(head, "Slash", 25, 6, 0);           // warrior
    addSkillToRole(head, "IronSkin", 8, 6, 1);         // buff def
    addSkillToRole(head->next, "PiercingShot", 20, 6, 0); // archer
    addSkillToRole(head->next, "RainOfArrows", 12, 12, 2);// archer AOE
    addSkillToRole(head->next->next, "Fireball", 30, 15, 0); // mage
    addSkillToRole(head->next->next, "HealingLight", 20, 10, 3); // mage heal

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
    while (p != nullptr) { c++; p = p->next; }
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
void addSkillToRole(RoleNode* role, const string &name, int value, int stamina, int type) {
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
void updateSkillInRole(RoleNode* role, int index, const string &name, int value, int stamina, int type) {
    SkillNode* s = findSkillByIndex(role, index);
    if (s == nullptr) return;
    s->name = name;
    s->value = value;
    s->staminaCost = stamina;
    s->type = type;
}

void printSkillsOfRole(RoleNode* role) {
    cout << "\nSkills of " << role->roleName << ":\n";
    SkillNode* p = role->skillHead;
    int i = 1;
    if (p == nullptr) cout << "(no skills)\n";
    while (p != nullptr) {
        cout << i << ". " << p->name
             << " | type=" << p->type
             << " | val=" << p->value
             << " | STA=" << p->staminaCost << "\n";
        p = p->next; i++;
    }
}

RoleNode* findRoleByIndex(RoleNode* head, int idx) {
    if (idx <= 0) return nullptr;
    RoleNode* p = head;
    int i = 1;
    while (p != nullptr && i < idx) { p = p->next; i++; }
    return p;
}

void printAllRoles(RoleNode* head) {
    cout << "\nRoles:\n";
    RoleNode* p = head;
    int i = 1;
    while (p != nullptr) {
        cout << i << ". " << p->roleName << " (skills: " << countSkills(p) << ")\n";
        p = p->next; i++;
    }
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
    addEnemyToStage(st1, "Goblin", 40, 8);

    // Stage 2
    addEnemyToStage(st1->next, "Wolf", 50, 10);
    addEnemyToStage(st1->next, "Wolf Pup", 25, 6);

    // Stage 3
    addEnemyToStage(st1->next->next, "Bandit", 55, 12);
    addEnemyToStage(st1->next->next, "Bandit Archer", 40, 10);

    // Stage 4
    addEnemyToStage(st1->next->next->next, "Skeleton", 60, 12);
    addEnemyToStage(st1->next->next->next, "Skeleton Archer", 35, 10);
    addEnemyToStage(st1->next->next->next, "Skeleton Knight", 75, 15);

    // Stage 5 - boss
    addEnemyToStage(st1->next->next->next->next, "Orc Chieftain (BOSS)", 180, 22);

    // Stage 6
    addEnemyToStage(st1->next->next->next->next->next, "Goblin Thief", 45, 9);
    addEnemyToStage(st1->next->next->next->next->next, "Goblin Bomber", 35, 14);

    // Stage 7
    addEnemyToStage(st1->next->next->next->next->next->next, "Dark Mage", 80, 18);

    // Stage 8
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band of Thieves A", 40, 10);
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band of Thieves B", 40, 10);
    addEnemyToStage(st1->next->next->next->next->next->next->next, "Band Leader", 60, 14);

    // Stage 9
    addEnemyToStage(st1->next->next->next->next->next->next->next->next, "Giant Spider", 120, 20);

    // Stage 10 - boss multi
    addEnemyToStage(st1->next->next->next->next->next->next->next->next->next, "Dragon Whelp", 90, 18);
    addEnemyToStage(st1->next->next->next->next->next->next->next->next->next, "Ancient Dragon (BOSS)", 300, 35);

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
    if (stage == nullptr) return;
    cout << "\nEnemies in Stage " << stage->stageNum << ":\n";
    EnemyNode* p = stage->enemyHead;
    int i = 1;
    while (p != nullptr) {
        cout << i << ". " << p->name << " | HP=" << p->hp << " | ATK=" << p->atk << "\n";
        p = p->next; i++;
    }
}

bool loginAdmin() {
    string user, pass;

    cout << "\n=== LOGIN ADMIN ===\n";
    cout << "Username : ";
    cin >> user;
    cout << "Password : ";
    cin >> pass;

    if (user == "admin" && pass == "admin123") {
        cout << "Login berhasil!\n";
        return true;
    }

    cout << "Username atau password salah!\n";
    return false;
}

#include "mll.h"

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

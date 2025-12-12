#ifndef MLL_H_INCLUDED
#define MLL_H_INCLUDED
#include <iostream>
#include <string>
using namespace std;

// =========================
// STRUCT SKILL (CHILD)
// =========================
struct Skill {
    string name;
    int damage;
    int stamina;
    string effect;
    Skill *next;
};

// =========================
// STRUCT ROLE (PARENT)
// =========================
struct Role {
    string name;
    Skill *skillHead;
    Role *next;
};

// =========================
// STRUCT ENEMY (CHILD)
// =========================
struct Enemy {
    string name;
    int hp;
    int damage;
    Enemy *next;
};

// =========================
// STRUCT STAGE (PARENT)
// =========================
struct Stage {
    int number;
    Enemy *enemyHead;
    Stage *next;
};

// =========================
// FUNGSI & PROSEDUR
// =========================

// init
void initRoles(Role* &roleHead);
void initStages(Stage* &stageHead);

// CRUD SKILL
void showSkills(Role* roleHead);
void addSkill(Role* roleHead);
void updateSkill(Role* roleHead);
void deleteSkill(Role* roleHead);

// Player
Role* chooseRole(Role* roleHead);
void playGame(Role* roleHead, Stage* stageHead);

// Login admin
bool loginAdmin();

#endif // MLL_H_INCLUDED

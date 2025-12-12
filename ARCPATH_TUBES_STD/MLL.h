#ifndef MLL_H
#define MLL_H

// WARNA
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"

#define BBLACK   "\033[90m"
#define BORANGE "\033[1;38;5;208m"
#define BRED     "\033[91m"
#define BGREEN   "\033[92m"
#define BYELLOW  "\033[93m"
#define BBLUE    "\033[94m"
#define BMAGENTA "\033[95m"
#define BCYAN    "\033[96m"
#define BWHITE   "\033[97m"
// background
#define BG_BYELLOW  "\033[103m"
#define BG_BRED     "\033[101m"
#define BG_BGRAY "\033[47m"
// TEXT STYLE
#define BOLD      "\033[1m"
#define DIM       "\033[2m"
#define ITALIC    "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK     "\033[5m"
#define REVERSE   "\033[7m"
#define HIDDEN    "\033[8m"


#include <iostream>
#include <string>
using namespace std;

/* -------------------------
   Skill node (child of Role)
   ------------------------- */
struct SkillNode {
    string name;
    int value;        // damage or buff amount or heal amount
    int staminaCost;
    string type;         // damage, defense, AOE, heal
    SkillNode* next;
};

/* -------------------------
   Role node (parent)
   Each role has pointer to head of SkillNode list
   ------------------------- */
struct RoleNode {
    string roleName;
    SkillNode* skillHead;
    RoleNode* next;
};

/* -------------------------
   Enemy node (child of Stage)
   ------------------------- */
struct EnemyNode {
    string name;
    int hp;
    int atk;
    EnemyNode* next;
};

/* -------------------------
   Stage node (parent)
   Each stage has pointer to head of EnemyNode list
   ------------------------- */
struct StageNode {
    int stageNum;
    EnemyNode* enemyHead;
    StageNode* next;
};

/* -------------------------
   MLL management functions (declarations)
   ------------------------- */

/* ---------------------------
   Helper functions for gameplay
   --------------------------- */

// find first alive enemy in enemy list
EnemyNode* firstAliveEnemy(EnemyNode* head);

// check if all enemies dead
bool allEnemiesDead(EnemyNode* head);

// apply damage to a single enemy node
void damageEnemy(EnemyNode* e, int dmg);

// enemies attack player; defense reduces flat damage
void enemiesAttack(EnemyNode* head, int &playerHp, int defense);

// print player and enemies
void printBattleStatus(int hp, int maxHp, int stamina, int maxStamina, int defense, StageNode* stage);

// copy skills from RoleNode into an indexed array for convenient choice
int copySkillsToArray(RoleNode* role, SkillNode* arr[], int maxArr);

/* ---------------------------
   Admin menu for CRUD operations
   --------------------------- */
void adminMenu(RoleNode* roles);

/* ---------------------------
   The battle loop per playthrough
   --------------------------- */
void playGame(RoleNode* rolesHead);

/* Role / Skill MLL */
RoleNode* createRoleList();                // create default roles parent list (Warrior, Archer, Mage)
void freeRoleList(RoleNode* head);         // free roles and their skill child lists

int countSkills(RoleNode* role);           // count skills for a role
SkillNode* findSkillByIndex(RoleNode* role, int idx); // find child skill by 1-based index

// admin CRUD
void addSkillToRole(RoleNode* role, const string &name, int value, int stamina, string type);
void removeSkillFromRole(RoleNode* role, int index);
void updateSkillInRole(RoleNode* role, int index, const string &name, int value, int stamina, string type);
void printSkillsOfRole(RoleNode* role);
RoleNode* findRoleByIndex(RoleNode* head, int idx); // returns pointer to RoleNode (1-based)

/* Stage / Enemy MLL */
StageNode* buildDefaultStages();           // create stages 1..10 with enemy child lists
void freeStages(StageNode* head);          // free stages and their enemies
StageNode* getStageByNumber(StageNode* head, int num); // find stage parent

// enemy child ops
void addEnemyToStage(StageNode* stage, const string &name, int hp, int atk);
int countEnemies(StageNode* stage);

/* Utility printing */
void printAllRoles(RoleNode* head);
void printStageEnemies(StageNode* stage);

bool loginAdmin();
void menu(RoleNode* roles);

#endif

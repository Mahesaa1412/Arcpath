#ifndef MLL_H
#define MLL_H

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
    int type;         // 0 = damage, 1 = buff(def), 2 = AOE, 3 = heal
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
/* Role / Skill MLL */
RoleNode* createRoleList();                // create default roles parent list (Warrior, Archer, Mage)
void freeRoleList(RoleNode* head);         // free roles and their skill child lists

int countSkills(RoleNode* role);           // count skills for a role
SkillNode* findSkillByIndex(RoleNode* role, int idx); // find child skill by 1-based index

// admin CRUD
void addSkillToRole(RoleNode* role, const string &name, int value, int stamina, int type);
void removeSkillFromRole(RoleNode* role, int index);
void updateSkillInRole(RoleNode* role, int index, const string &name, int value, int stamina, int type);
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

#endif








#include "mll.h"

// =====================================
// INISIALISASI ROLE & SKILL DEFAULT
// =====================================
void initRoles(Role* &roleHead) {
    Role *war = new Role{"Warrior", NULL, NULL};
    Role *arch = new Role{"Archer", NULL, NULL};
    Role *mage = new Role{"Mage", NULL, NULL};

    war->next = arch;
    arch->next = mage;

    // Warrior skills
    war->skillHead = new Skill{"Slash", 25, 5, "-", NULL};
    war->skillHead->next = new Skill{"Shield Bash", 10, 4, "-", NULL};
    war->skillHead->next->next = new Skill{"IronSkin", 0, 6, "DEF+", NULL};

    // Archer skills
    arch->skillHead = new Skill{"ArrowShot", 20, 4, "-", NULL};
    arch->skillHead->next = new Skill{"MultiShot", 15, 7, "AOE", NULL};

    // Mage skills
    mage->skillHead = new Skill{"Fireball", 30, 6, "-", NULL};
    mage->skillHead->next = new Skill{"ManaShield", 0, 4, "ReduceDMG", NULL};

    roleHead = war;
}

// =====================================
// INISIALISASI STAGE & ENEMY
// =====================================
void initStages(Stage* &stageHead) {
    stageHead = NULL;
    Stage* prev = NULL;

    for (int i=1; i<=10; i++) {
        Stage* st = new Stage{i, NULL, NULL};

        if (i == 5)
            st->enemyHead = new Enemy{"Ogre King (Boss)", 200, 18, NULL};
        else if (i == 10)
            st->enemyHead = new Enemy{"Dragon Emperor (FINAL BOSS)", 350, 25, NULL};
        else
            st->enemyHead = new Enemy{"Goblin", 40 + i*5, 5 + i, NULL};

        if (stageHead == NULL)
            stageHead = st;
        else
            prev->next = st;

        prev = st;
    }
}

// =====================================
// TAMPILKAN SEMUA SKILL
// =====================================
void showSkills(Role* roleHead) {
    Role* r = roleHead;

    while (r != NULL) {
        cout << "[" << r->name << "]\n";

        Skill* s = r->skillHead;
        int i = 1;

        while (s != NULL) {
            cout << i++ << ". " << s->name
                 << " | Dmg: " << s->damage
                 << " | Stamina: " << s->stamina
                 << " | Effect: " << s->effect << endl;
            s = s->next;
        }

        cout << endl;
        r = r->next;
    }
}

// =====================================
// TAMBAH SKILL
// =====================================
void addSkill(Role* roleHead) {
    int pilih;
    cout << "Tambah skill ke role:\n1. Warrior\n2. Archer\n3. Mage\n>> ";
    cin >> pilih;

    Role *r = roleHead;
    for (int i=1; i<pilih; i++) r = r->next;

    Skill* baru = new Skill;
    cout << "Nama skill: "; cin >> baru->name;
    cout << "Damage: "; cin >> baru->damage;
    cout << "Stamina: "; cin >> baru->stamina;
    cout << "Effect: "; cin >> baru->effect;
    baru->next = NULL;

    if (r->skillHead == NULL) r->skillHead = baru;
    else {
        Skill* t = r->skillHead;
        while (t->next != NULL) t = t->next;
        t->next = baru;
    }

    cout << "Skill berhasil ditambahkan!\n";
}

// =====================================
// UPDATE SKILL
// =====================================
void updateSkill(Role* roleHead) {
    showSkills(roleHead);

    int pilihRole;
    cout << "Update role mana?\n1. Warrior\n2. Archer\n3. Mage\n>> ";
    cin >> pilihRole;

    Role* r = roleHead;
    for (int i=1; i<pilihRole; i++) r = r->next;

    int pilihSkill;
    cout << "Skill nomor berapa? ";
    cin >> pilihSkill;

    Skill* s = r->skillHead;
    for (int i=1; i<pilihSkill; i++) s = s->next;

    cout << "Nama baru: "; cin >> s->name;
    cout << "Damage baru: "; cin >> s->damage;
    cout << "Stamina baru: "; cin >> s->stamina;
    cout << "Effect baru: "; cin >> s->effect;

    cout << "Skill berhasil diperbarui!\n";
}

// =====================================
// DELETE SKILL
// =====================================
void deleteSkill(Role* roleHead) {
    showSkills(roleHead);

    int pilihRole;
    cout << "Delete dari role:\n1. Warrior\n2. Archer\n3. Mage\n>> ";
    cin >> pilihRole;

    Role* r = roleHead;
    for (int i=1; i<pilihRole; i++) r = r->next;

    int pilihSkill;
    cout << "Hapus skill nomor: ";
    cin >> pilihSkill;

    Skill* s = r->skillHead;
    Skill* prev = NULL;

    for (int i=1; i<pilihSkill; i++) {
        prev = s;
        s = s->next;
    }

    if (prev == NULL)
        r->skillHead = s->next;
    else
        prev->next = s->next;

    delete s;

    cout << "Skill berhasil dihapus!\n";
}

// =====================================
// PLAYER MEMILIH ROLE
// =====================================
Role* chooseRole(Role* roleHead) {
    int pilih;
    cout << "Pilih Role:\n1. Warrior\n2. Archer\n3. Mage\n>> ";
    cin >> pilih;

    Role* r = roleHead;
    for (int i=1; i<pilih; i++) r = r->next;

    cout << "Kamu memilih: " << r->name << endl;
    return r;
}

// =====================================
// SISTEM PERTARUNGAN
// =====================================
void playGame(Role* roleHead, Stage* stageHead) {
    Role* job = chooseRole(roleHead);

    int hp = 120;
    int stamina = 40;

    Stage* st = stageHead;

    while (st != NULL) {
        cout << "\n===== STAGE " << st->number << " =====\n";

        Enemy* e = st->enemyHead;
        cout << "Musuh: " << e->name << " | HP: " << e->hp << endl;

        while (hp > 0 && e->hp > 0) {
            cout << "\nHP: " << hp << " | STAMINA: " << stamina << endl;

            Skill* s = job->skillHead;
            int i = 1;

            while (s != NULL) {
                cout << i++ << ". " << s->name << " (Dmg " << s->damage << ", Stm " << s->stamina << ")\n";
                s = s->next;
            }

            cout << i << ". Rest (+HP 5, +Stamina 8)\n";
            cout << "Aksi: ";
            int act;
            cin >> act;

            if (act == i) {
                hp += 5;
                stamina += 8;
                cout << "Kamu beristirahat...\n";
            } else {
                Skill* use = job->skillHead;
                for (int k=1; k<act; k++) use = use->next;

                if (stamina < use->stamina) {
                    cout << "Stamina tidak cukup!\n";
                    continue;
                }

                stamina -= use->stamina;
                e->hp -= use->damage;

                cout << "Kamu menggunakan " << use->name << "!\n";
                cout << "Musuh menerima " << use->damage << " damage!\n";
            }

            if (e->hp <= 0) break;

            cout << "\nGiliran musuh!\n";
            hp -= e->damage;
            cout << e->name << " menyerang! Kamu kena " << e->damage << " damage!\n";
        }

        if (hp <= 0) {
            cout << "\n===== KAMU KALAH =====\n";
            return;
        }

        cout << e->name << " dikalahkan!\n";
        hp += 10;
        stamina += 5;
        cout << "HP +10, Stamina +5\n";

        st = st->next;
    }

    cout << "\n===== SELAMAT! KAMU MENANG GAME =====\n";
}

// =====================================
// LOGIN ADMIN
// =====================================
bool loginAdmin() {
    string u, p;

    cout << "===== LOGIN ADMIN =====\n";
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    if (u == "admin" && p == "admin123") {
        cout << "Login berhasil!\n";
        return true;
    }

    cout << "Login gagal!\n";
    return false;
}

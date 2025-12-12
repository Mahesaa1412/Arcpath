
# ARCPATH — Turn-Based CLI Game (C++ / Multi-Linked List)

Arcpath adalah game turn-based berbasis Command Line Interface (CLI) yang dibangun menggunakan bahasa C++. Proyek ini mengimplementasikan Multi-Linked List (MLL) sebagai struktur data utama untuk mengelola role, skill, stage, dan enemy.

Game sederhana ini dirancang sebagai tugas besar untuk mendemonstrasikan MLL dalam bentuk aplikasi interaktif: pemain memilih role, menggunakan skill, melawan musuh per stage, sementara admin dapat mengelola skill dengan CRUD.

## > Tujuan Proyek

- Mengimplementasikan Multi-Linked List dalam konteks nyata

- Melatih penggunaan pointer & dynamic memory C++

- Mendesain game CLI dengan sistem CRUD

- Simulasi gameplay turn-based berbasis MLL
## > Sistem & Fitur Arcpath

```
📁 ARCPATH_TUBES_STD/
 ├── MLL.h
 ├── MLL.cpp
 ├── main.cpp
```

### 1. Struktur MLL

Proyek ini menggunakan struktur MLL:
- Role -> Skill List
- Stage -> Enemy List

### 2. Sistem Role
Tersedia 3 role default:
- Warrior
- Archer
- Mage

### 3. Sistem Stage & Enemy
Terdapat 10 stage:
- Stage 5 dan 10 berisi boss
- Setiap stage memiliki list musuh sendiri (MLL)
- Player harus mengalahkan semua musuh sebelum lanjut ke stage berikutnya

### 4. Gameplay
- Pemain dapat melakukan: Basic Attack, Use Skill, Rest
- Setelah player menyerang, musuh akan melakukan counter attack

### 5. Admin Panel
Admin dapat mengimplementasikan CRUD:
- Login sebagai admin
- Melihat skill sebuah role
- Menambah skill
- Mengupdate skill
- Menghapus skill

## > Authors 
Project ini dibuat secara berkelompok oleh 2 anggota yang berkontribusi:

- [@nisissy](https://github.com/nississy)
- [@Mahesaa1412](https://github.com/Mahesaa1412)

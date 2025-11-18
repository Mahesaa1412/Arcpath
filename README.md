# Arcpath
# Turn-Based RPG CLI Game

**C++ | Tree + Multi-Linked List (MLL)**

Game RPG berbasis CLI yang mengimplementasikan **struktur data Tree** untuk Skill Tree dan **Multi-Linked List (1-N)** untuk Inventory System. Pemain memilih role (Warrior, Archer, Mage), lalu dapat melihat skill tree unik masing-masing, mencari skill, dan mengakses inventory.

---

## Project Description

**Arcpath** adalah proyek game berbasis Command Line Interface yang dibangun menggunakan C++. Proyek ini dirancang untuk mendemonstrasikan implementasi struktur data **Tree (Left Child–Right Sibling)** dan **Multi Linked List (1-N)** melalui sistem skill tree dan inventory.

Game ini memberikan pengalaman bermain sederhana namun interaktif, di mana pemain dapat memilih role, melihat skill tree, mencari skill, dan mengelola inventory.

---

## ✨ Features

### 🌳 Skill Tree System (Tree)

* Setiap role memiliki skill tree sendiri.
* Representasi Tree menggunakan **Left Child – Right Sibling**.
* Setiap skill mempunyai:

  * `namaSkill`
  * `damage`
  * `staminaCost`
* Pencarian skill menggunakan DFS rekursif.

### 📦 Inventory System (MLL)

* Inventory disusun menggunakan Multi Linked List (1-N).
* Isi berupa berbagai jenis potion (Small, Medium, Elixir).
* Menampilkan dan menambah item ke inventory.

### 🎭 Role System

* **Warrior** – keseimbangan kekuatan.
* **Archer** – jarak jauh dengan skill bertingkat.
* **Mage** – magic damage besar.

---

## 🧩 Tech Stack

* C++
* Tree (Left Child – Right Sibling)
* Multi Linked List (1-N)
* Recursive Search
* CLI Interface

---

## 📂 Project Structure

```
📁 src/
 ├── skilltree.h
 ├── skilltree.cpp
 ├── inventory.h
 ├── inventory.cpp
 ├── main.cpp
```

---

## 🖼️ Skill Tree Example (Warrior)

```
Slash (DMG: 10 | ST: 5)
  ├── Heavy Slash (DMG: 20 | ST: 10)
  └── Whirlwind   (DMG: 35 | ST: 18)
```

---

## 🧪 How To Run

### Compile

```
g++ main.cpp skilltree.cpp inventory.cpp -o game
```

### Run

```
./game
```

---

## Authors
Project ini dibuat secara berkelompok oleh 2 anggota yang berkontribusi:

- [@nisissy](https://github.com/nississy) - Anisah Pulungan
- [@Mahesaa1412](https://github.com/Mahesaa1412) - Dimas Tri Mahesa

# 🌱 Plant-Inspired Optimization for Vertex Cover

Implementasi algoritma optimasi terinspirasi tumbuhan (PPA & IWO) untuk menyelesaikan masalah **Minimum Vertex Cover** pada graf.  
Proyek ini bagian dari tugas mata kuliah **Desain Algoritma Lanjut**.

---

## 📖 Pendahuluan

**Nature-Inspired Optimization Algorithms (NIOA)** adalah algoritma yang meniru fenomena alam untuk menyelesaikan masalah optimasi kompleks.  
Dua algoritma tumbuhan yang digunakan di sini:
- **Plant Propagation Algorithm (PPA)**: Meniru cara tanaman menyebar tunas untuk eksploitasi dan eksplorasi.
- **Invasive Weed Optimization (IWO)**: Meniru penyebaran dan adaptasi gulma dalam lingkungan baru.

---

## 🌳 Algoritma yang Diimplementasikan

### 🍓 Plant Propagation Algorithm (PPA)
- Tanaman “fit” menghasilkan banyak tunas pendek (eksploitasi lokal).
- Tanaman kurang “fit” menghasilkan sedikit tunas panjang (eksplorasi area baru).
- **File:** `ppa_vc.cpp`

### 🌾 Invasive Weed Optimization (IWO)
- Gulma yang adaptif menghasilkan lebih banyak benih.
- Benih tersebar acak di sekitar induk, jangkauan makin sempit tiap iterasi.
- Gulma terlemah dieliminasi jika populasi melebihi batas.
- **File:** `iwo_vc.cpp`

---

## 📊 Permasalahan: Vertex Cover

**Vertex Cover** adalah himpunan simpul sehingga setiap sisi graf memiliki minimal satu simpul dalam himpunan tersebut.  
Target: mencari cover seminimal mungkin (NP-hard).

---

## 📁 Struktur Proyek

- `ppa_vc.cpp`  →  Implementasi PPA
- `iwo_vc.cpp`  →  Implementasi IWO
- `graph.dat`   →  Contoh input graf
- `README.md`   →  Dokumentasi
- *(Opsional)* `Laporan_Akhir.pdf` → Laporan detail

---

## 💻 Teknologi yang Digunakan

- **Bahasa:** C++ (disarankan standar C++11 ke atas)
- **Kompiler:** g++ atau setara
- **Struktur Data:** Array C-style & std::pair
- **Tanpa** library eksternal

---

## ⚙️ Kompilasi & Eksekusi

### 📦 Prasyarat
- Kompiler C++ (g++ atau lain yang kompatibel)

### 🔨 Kompilasi

```bash
# PPA
g++ ppa_vc.cpp -o ppa_vertex_cover -std=c++11 -O2

# IWO
g++ iwo_vc.cpp -o iwo_vertex_cover -std=c++11 -O2

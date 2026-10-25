# RollCall 📋

RollCall is a lightweight student attendance management system built as a personal project to explore clean program design, user interaction, and data consistency. The application focuses on clarity, confirmation-based actions, and fixed attendance rules to avoid common errors in manual attendance tracking.

The project began as a console-based application and is designed to evolve into a GUI-driven tool.

---

## ✨ Features

- Add new students with:
  - Automatic roll number assignment
  - Stream selection
  - Full-name handling (with optional middle name)
  - **Confirmation summary before saving**
- Fixed attendance logic:
  - 5 subjects
  - 10 classes per subject (50 total)
- Subject-wise attendance marking
- Attendance percentage calculation
- Exam eligibility check (≥ 75%)
- Persistent storage using file handling
- Clear feedback for every action (no silent operations)

---

## 🧠 Design Philosophy

- **Safety over shortcuts**  
  User input is validated and confirmed before being saved.

- **Fixed rules, fewer bugs**  
  Total classes are constant to prevent inconsistent data.

- **User feedback matters**  
  Every important action provides a clear confirmation or error message.

- **Readable over clever**  
  The code is written to be understood and maintained easily.

---

## 🛠️ Tech Stack

- Language: **C** (core logic)
- Storage: File-based persistence
- UI:
  - Current: Console-based
  - Planned: GUI (Tkinter / GTK / Electron – personal exploration)

---

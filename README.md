MyCash
======

Simple console-based MyCash wallet demo (C++).

Overview
--------
- Small CLI app to register users, send/receive money, pay bills, and view transaction history.
- Member data is stored in `Members.txt`. Transaction history is appended to `history.txt`.

What I changed (Nov 23, 2025)
-----------------------------
- Persist `Members.txt` immediately after in-memory changes (registration, updates, removals, and transactions) so the file reflects the current state while the program is running. Previously the file was written only on program exit.

Build
-----
Requires a C++ compiler (g++ / MinGW recommended on Windows).
From PowerShell in the project folder run:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -o myCash.exe .\myCash.cpp
```

Run
---

```powershell
.\myCash.exe
```

Usage notes
-----------
- Mobile numbers must be 11 digits and start with `01`.
- Password input is hidden (asterisks) when entered.
- OTPs are printed to the console (this is a demo — do NOT use this approach in production).
- `Members.txt` columns: `Moble no\tUsername\tAmount\tPin` (tab-separated). Removed members are marked via the `uExist` flag; the file currently writes all users — see "Customization" below.

Files
-----
- `myCash.cpp` — main source code.
- `Members.txt` — stored members (tab-separated).
- `history.txt` — appended transaction history (each transaction written immediately when performed).

Customization ideas
-------------------
- Exclude removed members from `Members.txt` (filter by `uExist`).
- Flush/commit file handles for stronger durability guarantees.
- Replace console OTP with an SMS/email integration.
- Use a proper storage format (JSON/SQLite) for safer parsing and concurrency.

Testing
-------
- Register a user, then open `Members.txt` in a text editor while the program is still running — the new member should appear immediately after registration.
- Perform cash-in / send-money operations and verify the `Amount` column updates right away.

Contact
-------
For further changes or to add features, update `myCash.cpp` or open an issue in your repository.
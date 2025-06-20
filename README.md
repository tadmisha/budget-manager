(ChatGPT generated)

# 💰 Budget Manager

A simple terminal-based Budget Manager written in C++ that allows users to track income, expenses, and view transaction history. User data is saved in a JSON file using the [nlohmann/json](https://github.com/nlohmann/json) library.

## 📁 Project Structure

```
budget-manager/
├── include/               # Header files (e.g. json.hpp)
├── src/                   # Source files (main.cpp)
├── users.json             # Persistent storage for users
├── README.md              # Project description
└── .gitignore             # Git ignore file
```

## 🚀 Features

- User sign-up and login
- Track income and expenses
- View transaction history with timestamps
- Persistent data storage in `users.json`
- Simple terminal-based UI

## 🛠️ Requirements

- C++17 or newer
- [nlohmann/json](https://github.com/nlohmann/json) — single-header JSON library

## 🧑‍💻 How to Build

```bash
g++ src/main.cpp -o budget-manager -I include
```

## ▶️ How to Run

```bash
./budget-manager
```

## ✏️ Available Commands

After signing in:

- `help` — show list of commands
- `balance` — check current balance
- `earn` — add income
- `spend` — subtract expense
- `history` — view transaction history
- `logout` — log out of the account
- `exit` — exit the program

## 🧾 Example JSON Entry

```json
{
  "0": {
    "id": 0,
    "username": "example",
    "name": "John",
    "surname": "Doe",
    "email": "john@example.com",
    "password": "securepass",
    "budget": 250.5,
    "transactions": [
      {
        "amount": 100.0,
        "time": "Thu Jun 20 18:30:00 2025"
      }
    ]
  }
}
```

## 📌 Notes

- Put `json.hpp` in the `include/` folder.
- Only standard C++ and the JSON library are used.
- You can compile and run on any platform with a modern C++ compiler.

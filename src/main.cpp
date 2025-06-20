#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <cctype>
#include "json.hpp"

using json = nlohmann::json;

//& Checks if the string is double
bool is_double(const std::string& str) {
    try {
        std::size_t pos;
        std::stod(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

//& Converts chrono time to string
std::string time_to_str(std::chrono::time_point<std::chrono::system_clock> time) {
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::string stime = std::ctime(&t);
    stime.pop_back();
    return stime;
}


// & Class storing transaction info
struct Transaction {
    const double amount;
    const std::string time;

    Transaction (double amount_, const std::string& time_):
    amount(amount_), time(time_) {
        std::cout << "Transaction made at " << time << std::endl;
    }
};


// & Class storing user's budget
class Budget {
    double budget;
    
    public:
        std::vector<Transaction> transaction_history;

        Budget (double budget_ = 0): 
        budget(budget_) {
            std::cout << "Budget established at: " << budget << '$' << std::endl;
        }

        void transact(double amount) { 
            auto time = std::chrono::system_clock::now();
            budget+=amount;
            transaction_history.push_back(Transaction(amount, time_to_str(time)));
        }

        double get_budget() const { return budget; }
};


// & Class with user's data
class User {
    static int last_id;

    std::string username;
    std::string name;
    std::string surname;
    std::string email;
    std::string password;
    const int user_id;

    //& Function to figure out the correct user id
    static int get_id(int user_id_) {
        if (user_id_==-1) {
                last_id++;
                return last_id;
            }
        return user_id_;
    }

    public:
        Budget budget;

        User (const std::string& username_, const std::string& name_, const std::string& surname_, const std::string& email_, const std::string& password_, double budget_=0, int user_id_ = -1): 
        user_id(get_id(user_id_)), username(username_), name(name_), surname(surname_), email(email_), password(password_), budget(Budget(budget_)) {
            std::cout << username << " user initialized." << std::endl << std::endl;
        }

        int get_id() const { return user_id; }
        std::string get_username() const { return username; }
        std::string get_name() const { return name; }
        std::string get_surname() const { return surname; }
        std::string get_email() const { return email; }
        std::string get_password() const { return password; }
        double get_budget() const {return budget.get_budget(); }

        static void change_last_id(int new_last_id) { last_id = new_last_id; }
};



int User::last_id = -1;



//& Function to find specific user by their username
User* find_user(const std::string& username, std::vector<User>& users) {
    for (auto& user : users) {
        if (user.get_username() == username) {
            return &user;
        }
    }

    return nullptr;
}


// & Loads users from a json file
std::vector<User> load_users(const std::string& filename = "users.json") {
    std::vector<User> users;
    std::ifstream file(filename);
    
    json users_json;
    file >> users_json;

    file.close();

    int max_id = -1;

    for (const auto& user_json : users_json) {
        users.push_back(User(user_json["username"], user_json["name"], user_json["surname"], user_json["email"], user_json["password"], user_json["budget"], user_json["id"]));
        int tr_i = 0;
        for (const auto& transcation : user_json["transactions"]) {
            users.back().budget.transaction_history.push_back(Transaction(user_json["transactions"][tr_i]["amount"], user_json["transactions"][tr_i]["time"]));
            tr_i++;
        }

        if (user_json["id"] > max_id) {
            max_id = user_json["id"];
        }
    }

    User::change_last_id(max_id);

    std::cout << std::endl << std::endl << std::endl;


    return users;
    
}


//& Uploads users to a json file
void upload_users(const std::vector<User>& users, const std::string& filename = "users.json") {
    json users_json;
    for (const auto user : users) {
        const int id = user.get_id();
        users_json[id]["id"] = id;
        users_json[id]["username"] = user.get_username();
        users_json[id]["name"] = user.get_name();
        users_json[id]["surname"] = user.get_surname();
        users_json[id]["email"] = user.get_email();
        users_json[id]["password"] = user.get_password();
        users_json[id]["budget"] = user.get_budget();

        users_json[id]["transactions"] = {};
        
        for (int tr_i = 0; tr_i < user.budget.transaction_history.size(); tr_i++) {
            users_json[id]["transactions"][tr_i]["amount"] = user.budget.transaction_history[tr_i].amount;
            users_json[id]["transactions"][tr_i]["time"] = user.budget.transaction_history[tr_i].time;
        }
    }

    std::ofstream file(filename);
    file << users_json.dump(4);
    file.close();
}


// & Asks for data and creates a new user
void sign_up(std::vector<User>& users) {
    std::string name;
    std::string surname;
    std::string username;
    std::string email;
    std::string password;

    std::string full_name;
    std::string password2;

    std::cout << "Input your full name: ";
    std::getline(std::cin, full_name);
    std::cout << std::endl;

    do {
        std::cout << "Input your username: ";
        std::cin >> username;
        if (!find_user(username, users)) {  break;  }
        std::cout << "Username taken, try another one." << std::endl;
    } while (true);
    std::cout << std::endl;

    std::cout << "Input your email: ";
    std::cin >> email;
    std::cout << std::endl;

    std::cout << "Input your password: ";
    std::cin >> password;
    std::cout << std::endl;

    do {
        std::cout << "Repeat the password: ";
        std::cin >> password2;
        std::cout << std::endl;
    } while (!(password == password2));

    name = "";
    surname = "";
    bool before_space = true;
    for (int i=0; i < full_name.size(); i++) {
        if (full_name[i] == ' ') {
            before_space = false;
            continue;
        }

        if (before_space) {
            name+=full_name[i];
        }

        else {
            surname+=full_name[i];
        }
    }

    users.push_back(User(username, name, surname, email, password));
}



int main() {
    std::vector<User> users;
    users = load_users();

    std::string income_expense_str;
    double income_expense;

    char in_or_up; //* Stores 1 - sign in, 2 - sign up

    User* user = nullptr;
    std::string username;
    std::string password;

    std::string command;

    bool run = true;

    while (run) {
        // ? Sign in/up if not signed in
        if (!user) {
            std::cout << "Welcome to our budget manager, type 1 to sign in, type 2 to sign up: ";
            // ^ Asks if user wants to sign in or up
            do {
                std::cin >> in_or_up;
                if ((in_or_up=='1') | (in_or_up=='2')) { break; }
                std::cout << "Invalid input, type 1 to sign in, type 2 to sign up: ";
            } while (true);
            
            std::cout << std::endl;

            //^ If sign in
            if (in_or_up=='1') {
                std::cout << "Sign in to start." << std::endl;
                
                do {
                    std::cout << "Input your username: ";
                    std::cin >> username;
                    
                    user = find_user(username, users);
                    if (user) { break; }

                    std::cout << "Username not found, try again." << std::endl;
                } while (true);
                
                do {
                    std::cout << "Input your password: ";
                    std::cin >> password;
                    
                    if (user->get_password() == password) { break; }

                    std::cout << "Password incorrect, try again." << std::endl;
                } while (true);

                std::cout << "Welcome back " << username << '!' << std::endl << "Type \"help\" to learn all the commands." << std::endl << std::endl;

            }

            //^ If sign up
            else if (in_or_up=='2') {
                std::cout << "Sign up." << std::endl;
                sign_up(users);
                std::cout << "Signed up successfully";
                upload_users(users);
            }
            continue;
        }

        std::cout << "Input a command: ";
        std::cin >> command;


        //? help - show available commands
        if (command == "help") {
            std::cout << "help - show available commands" << std::endl
            << "balance - show current balance" << std::endl
            << "earn - add income to the balance" << std::endl
            << "spend - subtract expense from the balance" << std::endl
            << "history - show transaction history" << std::endl
            << "logout - log out from your account" << std::endl
            << "exit - exit your beloved budget manager" << std::endl << std::endl;
        
        }

        //? balance - show current balance
        else if (command == "balance") {
            std::cout << "Your current balance is " << user->get_budget() << '$' << std::endl << std::endl;
        }

        //? earn - add income to the balance
        else if (command == "earn") {
            std::cin.ignore();
            do {
                    std::cout << "Input your income: ";
                    std::cin >> income_expense_str;
                    
                    if (is_double(income_expense_str)) { 
                        income_expense = std::stod(income_expense_str);
                        break;
                    }

                    std::cout << "Input incorrect, try again." << std::endl;
                } while (true);
            user->budget.transact(income_expense);
            std::cout << income_expense << "$ added to the account" << std::endl << std::endl;

            upload_users(users);
        }

        //? spend - subtract expense from the balance
        else if (command == "spend") {
            do {
                    std::cout << "Input your expense: ";
                    std::cin >> income_expense_str;
                    
                    if (is_double(income_expense_str)) { 
                        income_expense = std::stod(income_expense_str);
                        break;
                    }

                    std::cout << "Input incorrect, try again." << std::endl;
                } while (true);
            user->budget.transact(-income_expense);
            std::cout << income_expense << "$ removed from the account" << std::endl << std::endl;

            upload_users(users);
        }
        
        //? history - show transaction history
        else if (command == "history") {
            for (const auto& transaction : user->budget.transaction_history) {
                std::cout << transaction.amount << "$ " << transaction.time << std::endl;
            }
            std::cout << std::endl;
        }

        //? logout - log out from the account
        else if (command == "logout") {
            user = nullptr;
            std::cout << "Logged out of account. " << std::endl << std::endl;

            std::cout << std::endl;
        }

        //? exit
        else if (command == "exit") {
            std::cout << "Goodbye. See you next time" << std::endl;
            break;
        }

        //? Invalid command
        else {
            std::cout << "Unknown command, try again" << std::endl << std::endl;
        }

    }

    return 0;
}
#include "login.hpp"
#include "email.hpp"
#include <fstream>

// Global Variables
const char *databaseName = "databases/GeeMail.db";
sql_driver mysql_driver;
login_package user;
// Progression flags
bool correct_login = false;
bool logged_in = false;
bool viewing_inbox = false;
bool quit = false;

// Gets up to 2 input numbers from the user and returns them as ints
// Mades sure the the input is actaully an number, otherwise, -1 is returned
int get_input_number(){
    string input;
    getline(cin, input);
    
    // Only accepts 1 or 2 numbers as input
    if(input.length() == 0 || input.length() >= 3){
        return -1;
    }
    
    // Check each input character to make sure it is a number
    for(int i = 0; i < input.length(); i++){
        if(input.at(i) < 48 || input.at(i) > 57){
            return -1;
        }
    }
    
    return stoi(input);
}

void sanitize_user(){
    user.username.clear();
    user.password.clear();
    user.correct_format = false;
}

void home_page(){
    int user_input = -1;
    cout << "What would you like to do?" << endl << "1. Login" << endl << "2. Register" << endl << "0. Quit Program" << endl;
    user_input = get_input_number();
    if(user_input == 1){
        user = attempt_login(mysql_driver, "Users");
        if(user.correct_format){
            correct_login = true;
        }
        else{
            correct_login = false;
            sanitize_user();
        }
    }
    else if(user_input == 2){
        correct_login = false;
        attempt_register(mysql_driver, "Users");
    }
    else if(user_input == 0){
        correct_login = false;
        quit = true;
    }
    else{
        correct_login = false;
        cout << "Invalid command" << endl;
    }
}

// instructions once inbox is shown; for inbox variable, 1 is new only, 2 is all
void inbox_control(int inbox){
    int user_input = -1;
    cout << "What would you like to do?" << endl << "1. Read a message" << endl << "2. Delete a message" << endl << "3. Return to previous menu" << endl;
    user_input = get_input_number();
    if(user_input == 1){
        cout << "Which email would you like to read? ";
        int email_number = get_input_number();
        read_email(mysql_driver, user.username, inbox, email_number);
    }
    else if(user_input == 2){
        cout << "Which email would you like to delete? ";
        int email_number = get_input_number();
        delete_email(mysql_driver, user.username, inbox, email_number);
    }
    else if(user_input == 3){
        viewing_inbox = false;
    }
    else{
        cout << "Invalid command" << endl;
    }
}

void user_home(){
    int user_input = -1;
    cout << "What would you like to do?" << endl << "1. Check inbox" << endl << "2. Write a message" << endl << "3. Log out" << endl << "0. Quit Program" << endl;
    user_input = get_input_number();
    if(user_input == 1){
        cout << endl << "1. View inbox (new)" << endl << "2. View inbox (all)" << endl;
        int message_choice = get_input_number();
        viewing_inbox = true;
        while(viewing_inbox){
            if(message_choice == 1){
                display_inbox_new(mysql_driver, user.username);
                inbox_control(message_choice);
            }
            else if(message_choice == 2){
                display_inbox_all(mysql_driver, user.username);
                inbox_control(message_choice);
            }
            else{
                cout << "Invalid command!" << endl;
                viewing_inbox = false;
            }
        }
    }
    else if(user_input == 2){
        send_email(mysql_driver, user.username);
    }
    else if(user_input == 3){
        logged_in = false;
        correct_login = false;
    }
    else if(user_input == 0){
        logged_in = false;
        correct_login = false;
        quit = true;
    }
    else{
        cout << "Invalid command" << endl;
    }
}

void printImage(){
    ifstream in("squid.txt");
    if(!in){
        return;
    }
    string line;
    while(getline(in, line)){
        cout << line << endl;
    }
    
    in.close();
    return;
}

int main(){
    /* open database */
    mysql_driver.open_database(databaseName);
    
    /*Making a table, if it doesn't alreadt exist*/
    mysql_driver.make_users_table("Users", "username", "password", "salt");
    vector<bucket8_t> users;
    
    for(int i = 0; i < users.size(); i++){
        cout << users.at(i).val0 << endl;
    }

    /* Main operation */
    while(not quit){
        /* "Home page" */
        home_page();
        
        if(correct_login){
            logged_in = true;
            cout << "Welcome back " << user.username << endl;
        }
        
        while(logged_in){
            user_home();
        }
    }
    
    cout << "Closing GeeMail..." << endl << "Brought to you by: " << endl;
    mysql_driver.close_database();
    printImage();
    
    return 0;
}
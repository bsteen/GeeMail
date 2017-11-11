#include "login.hpp"
#include "sqlite3_driver.hpp"

// Global Variables
sql_driver mysql_driver;
// Progression flagss
bool correct_login = false;
bool logged_in = false;
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

void home_page(){
    int user_input = -1;
    cout << "What would you like to do?" << endl << "1. Login" << endl << "2. Register" << endl << "0. Quit Program" << endl;
    user_input = get_input_number();
    if(user_input == 1){
        attempt_login();
        
        // login = request_input();
        // login_user(login);
    }
    else if(user_input == 2){
        attempt_register();
        // register
    }
    else if(user_input == 0){
        quit = true;
    }
    else{
        cout << "Invalid command" << endl;
    }
}

void login_user(login_package login){
    if(login.correct_format){
        vector<bucket8_t> table_information;
        mysql_driver.conditional_search("Users", "username", login.username, table_information);
        if(table_information.size() == 1){
            verify_password(login, )
            // encrypt password input here
            if(login.password.compare(table_information.at(0).val1) == 0){
                correct_login = true;
            }
            else{
                cout << "Login information incorrect" << endl;
            }
        }
        else{
            cout << "Login information incorrect" << endl;
        }
    }
}

int main(){
    /* Make the driver */
    const char *databaseName = "databases/GeeMail.db";
    int rc;
    string sqlcmd;
    
    /* open database */
    mysql_driver.open_database(databaseName);
    
    /*Making a table*/
    mysql_driver.make_users_table("Users", "username", "password", "salt");
    
    /* Add test user */
    User_t test;
    test.username = "casey";
    test.password = "cvorgcvorg";
    test.salt = "1337";
    mysql_driver.insert_user("Users", test);
    
    /* Main operation */
    while(not quit){
        login_package login;
        /* "Home page" */
        while(not correct_login){
            home_page();
        }
        logged_in = true;
        cout << "Welcome back " << login.username << endl;
        while(logged_in){
            string instruction;
            cout << "What would you like to do?" << endl << "1. Check messages" << endl << "2. Write a message" << endl << "3. Log out" << "0. Quit Program" << endl;
            if(instruction == "3"){
                logged_in = false;
                correct_login = false;
            }
        }
    }
    
    cout << "Closing GeeMail" << endl;
    return 0;
}
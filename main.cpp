#include "login.hpp"
#include "encrypt.hpp"
#include "sqlite3_driver.hpp"

int main(){
    /* Make the driver */
    sql_driver mysql_driver;
    const char * databaseName;
    databaseName = "databases/GeeMail.db";
    int rc;
    string sqlcmd;
    
    /* progression flags */
    bool correct_login = false;
    bool logged_in = false;
    
    /* open database */
    mysql_driver.open_database(databaseName);
    /*Making a table*/
    mysql_driver.make_users_table("Users", "username", "password", "salt");
    User_t test;
    test.username = "casey";
    test.password = "cvorgcvorg";
    test.salt = "1337";
    mysql_driver.insert_user("Users", test);
    while(1){
        login_package login;
        while(not correct_login){
            login = request_input();
            if(login.correct_format){
                vector<bucket8_t> table_information;
                mysql_driver.conditional_search("Users", "username", login.username, table_information);
                if(table_information.size() == 1){
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
        logged_in = true;
        cout << "Welcome back " << login.username << endl;
        while(logged_in){
            string instruction;
            cout << "What would you like to do?" << endl << "1. Check messages" << endl << "2. Write a message" << endl << "3. Log out" << endl;
            getline(cin, instruction);
            if(instruction == "3"){
                logged_in = false;
                correct_login = false;
            }
        }
    }
}
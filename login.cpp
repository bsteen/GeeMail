#include "login.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

login_package attempt_login(sql_driver &driver, string tableName){
    // sql_driver driver;
    // driver.open_database(databaseName);
    login_package user_info = request_input();
    bool login_correct = false;
    
    if(user_info.correct_format){
        lowercase_convert(user_info.username);
        vector<bucket8_t> table_information;
        driver.conditional_search("Users", "username", user_info.username, table_information);
        
        if(table_information.size() == 1){
            user_info.password = encrypt_password(user_info.password, table_information.at(0).val2);
            
            if(user_info.password.compare(table_information.at(0).val1) == 0){
                login_correct = true;
            }
            else{
                cout << "Login information incorrect" << endl;
            }
        }
        else{
            cout << "Login information incorrect" << endl;
        }
    }
    user_info.correct_format = login_correct;
    // driver.close_database();
    return user_info;
}

void attempt_register(sql_driver &driver, string tableName){
    // sql_driver driver;
    // driver.open_database(databaseName);
    cout << "Please enter new username and password" << endl;
    login_package user_info = request_input();
    if(user_info.correct_format){
        lowercase_convert(user_info.username);
        
        User_t new_user;
        new_user.username = user_info.username;
        new_user.salt = generate_salt();
        new_user.password = encrypt_password(user_info.password, new_user.salt);
        
        driver.insert_user(tableName, new_user);
        driver.make_email_table(new_user.username + "Emails", "receiver", "sender", "time_stamp", "message", "read");
    }
    else{
        cout << "Failed to create user" << endl;
    }
    // driver.close_database();
}

login_package request_input(){
    size_t bytes = 30;
    string user_in, pass_in;
    int user_count, pass_count;
    
    cout << "Enter username: " << endl;
    getline(cin, user_in);

    cout << "Enter password: " << endl;
    getline(cin, pass_in);
    
    login_package login_attempt;

    if(valid_credentials(user_in, pass_in, bytes)){
        // cout << "Username and password are good" << endl;
        login_attempt.username = user_in;
        login_attempt.password = pass_in;
        login_attempt.correct_format = true;
        return login_attempt;
    }
    else{
        cout << "Please input valid credentials" << endl;
        return login_attempt;
    }
}

bool valid_credentials(string username, string password, size_t bytes){       // basic input checking that searches for invalid characters (ie spaces, semicolons, etc.)
    if((username.empty()) or (password.empty()) or (username.at(0) == ' ') or (password.at(0) == ' ') or (username.length() > bytes) or (password.length() > bytes)){     // usernames and passwords cannot start with spaces
        return false;
    }
    for (int i=0; i < username.length(); i++){
        int character = (int)username.at(i);
        if((character < 48) or (character > 57 and character < 65) or (character > 90 and character < 97) or (character > 122)){  // usernames cannot contain special characters or spaces
            return false;
        }
    }
    for (int i=0; i < password.length(); i++){
        int character = (int)password.at(i);
        if((character < 32) or (character == 59) or (character > 126)){  // password must use valid keyboard characters and cannot contain semicolon
            return false;
        }
    }
    return true;
}

void lowercase_convert(string &word){
    for(int i = 0; i < word.length(); i++){
        word.at(i) = tolower(word.at(i));
    }
}
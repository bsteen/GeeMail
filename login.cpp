#include "login.hpp"
#include "encrypt.hpp"
#include "sqlite3_driver.hpp"

login_package attempt_login(){
    
}

void attempt_register(){
    login_package user_info = request_input();
    
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


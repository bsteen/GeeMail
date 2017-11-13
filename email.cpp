#include "email.hpp"
#include "encrypt.hpp"
#include <ctime>

// Prints the "header" information of an email in a nice format
// Message number, sender, time stamp, READ/UNREAD
void print_email_header(int message_num, Message_t message){
    string line = "|";
    // Message num
    if(message_num < 10){
        line += " ";
    }
    line += to_string(message_num) + "|";
    
    // sender
    int username_space = 12;
    if(message.sender.length() > 12){
        line = line + message.sender.substr(0, 9) + "..." + "|";
    }
    else{
        line += message.sender;
        for(int i = 0; i < username_space - message.sender.length(); i++){
            line += " ";
        }
        line += "|";
    }
    
    // timestamp
    line = line + message.time_stamp.substr(0, message.time_stamp.length() - 1) + "|";
    
    // read/unread
    if(message.read == 0){
        line += "    No |";
    }
    else{
        line += "   Yes |";
    }
    cout << line << endl;
    cout << "|------------------------------------------------|" << endl;
    return;
}

// Prints out a numbered list of emials in a user's inbox
void display_inbox_all(sql_driver &driver, string username){
    vector<bucket8_t> messages;
    string TableName = username + "Emails";
    driver.conditional_search(TableName,"receiver",username,messages);
    cout <<"Welcome "<< username <<", this is your whole inbox: "<< endl;
    cout <<"| #|   SENDER   |          TIME          | READ? |"<<endl;
    cout << "|------------------------------------------------|" << endl;
    Message_t email;
    for (int i = 0; i<messages.size();i++){
        email.receiver = messages.at(i).val0;
        email.sender = messages.at(i).val1;
        email.time_stamp = messages.at(i).val2;
        email.message = messages.at(i).val3;
        email.read = stoi(messages.at(i).val4);
        print_email_header(i,email);
    }
    return;
}

// Display only the unread ("new") messages in a user's inbox
void display_inbox_new(sql_driver &driver, string username){
    vector<bucket8_t> messages;
    string TableName = username + "Emails";
    driver.conditional_search(TableName,"receiver",username,messages);
    cout <<"Welcome "<< username <<", this is your unread inbox: "<< endl;
    cout <<"| #|   SENDER   |          TIME          | READ? |"<<endl;
    cout << "|------------------------------------------------|" << endl;
    Message_t email;
    int new_message_counter = 0;
    for (int i = 0; i<messages.size();i++){
        if(messages.at(i).val4.compare("0") == 0){
            email.receiver = messages.at(i).val0;
            email.sender = messages.at(i).val1;
            email.time_stamp = messages.at(i).val2;
            email.message = messages.at(i).val3;
            email.read = stoi(messages.at(i).val4);
            print_email_header(new_message_counter,email);
            new_message_counter++;
        }
    }
    return;
}

// Attempt to decrypt and display a selected email
void read_email(sql_driver &driver, string username, int inbox, int email_number){
    
    // Check if email exists in database
    vector<string> email_list;
    vector<string> timestamp_list;
    bool valid_index = false;
    
    if(email_number < 99 && email_number >= 0){
        vector<bucket8_t> messages;
        string TableName = username + "Emails";
        driver.conditional_search(TableName,"receiver",username,messages);
            
        if(inbox == 1){ // Index is for unread emails
            int new_message_counter = 0;
            for (int i = 0; i<messages.size();i++){
                if(messages.at(i).val4.compare("0") == 0){
                    timestamp_list.push_back(messages.at(i).val2);
                    email_list.push_back(messages.at(i).val3);
                    new_message_counter++;
                }
            }
            if(email_number < new_message_counter){
                valid_index = true;
            }
        }  
        else{
            if(email_number < messages.size()){
                valid_index = true;
                for (int i = 0; i<messages.size();i++){
                    timestamp_list.push_back(messages.at(i).val2);
                    email_list.push_back(messages.at(i).val3);
                }
            }
        }
    }
    
    if(valid_index){
        string password;
        string encrypted_message = email_list.at(email_number);
        // Ask user for shared password
        cout << "Enter shared password to decrypt message: ";
        getline(cin, password);
        
        
        // Decrypt message from email and display it
        cout << "---BEGIN MESSAGE---" << endl;
        cout << decrypt_message(encrypted_message, password) << endl;
        cout << "---END MESSAGE---" << endl;
        
        // Mark email as read
        driver.update_entry(username + "Emails", "read", "time_stamp", timestamp_list.at(email_number), "1");
        
        password.clear();
        encrypted_message.clear();
        return;
    }
    else{
        cout << "Email not found in inbox" << endl;
        return;
    }
}

// Check if the username entered for the receiver is valid
bool valid_username(sql_driver &driver, string &username){
    if((username.empty()) or (username.at(0) == ' ') or (username.length() > 30)){     // usernames and passwords cannot start with spaces
        return false;
    }
    for (int i=0; i < username.length(); i++){
        int character = (int)username.at(i);
        if((character < 48) or (character > 57 and character < 65) or (character > 90 and character < 97) or (character > 122)){  // usernames cannot contain special characters or spaces
            return false;
        }
    }
    for(int i = 0; i < username.length(); i++){
        username.at(i) = tolower(username.at(i));
    }
    vector<bucket8_t> recipient;
    driver.conditional_search("Users", "username", username, recipient);
    if(recipient.size() == 1){
        return true;
    }
    return false;
}

// Send an encrypted email to another user
void send_email(sql_driver &driver, string sender){
    // Have user enter recipient username
    string user_in;
    cout << "Enter receiver username: " << endl;
    getline(cin, user_in);
    // Check if user name exists
    if(valid_username(driver, user_in)){
        string tableName = user_in + "Emails";
        string raw_message;
        string password;
        string encrypted_message;
        
        // User composes email (a blank line as input means they are done editing)
        cout << "Write message below:" << endl;
        getline(cin, raw_message);
        
        if(raw_message.length() > 4000){
            cout << "Email too long; keep under 4000 characters. Email not sent!" << endl;
            raw_message.clear();
            return;
        }
        
        // User enters in shared password
        cout << "Enter shared password:" << endl;
        getline(cin, password);
        
        // Encrypt email with shared password
        encrypted_message = encrypt_message(raw_message, password);
        
        // Get time stamp for email
        time_t rawtime;
        time(&rawtime);
        string timestamp = ctime(&rawtime);
        
        // "Send email" by storing it in the receiver's database entry
        Message_t email;
        email.receiver = user_in;
        email.sender = sender;
        email.message = encrypted_message;
        email.time_stamp = timestamp;
        email.read = 0;
        driver.insert_email(tableName, email);
        
        raw_message.clear();
        password.clear();
        encrypted_message.clear();
        timestamp.clear();
        
        cout << "Message sent!" << endl;
        
        return;
    }
    cout << "No such user with that username" << endl;
    return;
}

// Delete an email for a user's inbox
void delete_email(sql_driver &driver, string username, int inbox, int email_number){

    // Check if email exists in database
    vector<string> timestamp_list;
    bool valid_index = false;
    string TableName = username + "Emails";
    
    if(email_number < 99 && email_number >= 0){
        vector<bucket8_t> messages;
        driver.conditional_search(TableName,"receiver",username,messages);
            
        if(inbox == 1){ // Index is for unread emails
            int new_message_counter = 0;
            for (int i = 0; i<messages.size();i++){
                if(messages.at(i).val4.compare("0") == 0){
                    timestamp_list.push_back(messages.at(i).val2);
                    new_message_counter++;
                }
            }
            if(email_number < new_message_counter){
                valid_index = true;
            }
        }  
        else{
            if(email_number < messages.size()){
                valid_index = true;
                for (int i = 0; i<messages.size();i++){
                    timestamp_list.push_back(messages.at(i).val2);
                }
            }
        }
    }
    
    if(valid_index){
        string response;
        cout << "Are you sure you want to delete this message? (y/n)" << endl;
        getline(cin, response);
        
        if(response.compare("y") == 0){
            // Delete email entry from database
            driver.delete_entry(TableName, "time_stamp", timestamp_list.at(email_number));
            cout << "Message deleted!" << endl;
            return;
        }
        else{
            cout << "Message NOT deleted!" << endl;
            return;
        }
    }
    else{
        cout << "You can't delete an email that's not there" << endl;
        return;
    }
}
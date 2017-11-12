#include "email.hpp"

// Prints out a numbered list of emials in a user's inbox
// Shows: #: <read/unread> <email sender> <timestamp of when it was sent>
void display_inbox_all(sql_driver &driver, string username){
    vector<bucket8_t> messages;
    
}

void display_inbox_new(string username){
    
}

void read_email(int email_number){
    
    // Check if email exists in database
    
    // As use for shared password

    // See if shared passord is correct
    
    // Display email
    
    // Mark email as read
    
}

bool valid_username(sql_driver &driver, string username){
    if((username.empty()) or (username.at(0) == ' ') or (username.length() > 30)){     // usernames and passwords cannot start with spaces
        return false;
    }
    for (int i=0; i < username.length(); i++){
        int character = (int)username.at(i);
        if((character < 48) or (character > 57 and character < 65) or (character > 90 and character < 97) or (character > 122)){  // usernames cannot contain special characters or spaces
            return false;
        }
    }
    vector<bucket8_t> recipient;
    driver.conditional_search("Users", "username", username, recipient);
    if(recipient.size() == 1){
        return true;
    }
    
    return false;
}

void send_email(sql_driver &driver, string sender){
    // Have user enter recipient username
    string user_in;
    cout << "Enter username: " << endl;
    getline(cin, user_in);
    // Check if user name exists
    if(valid_username(driver, user_in)){
        // Add sender and receiver to email
        cout << "Cool now I have to implement password input" << endl;
        // User composes email (a blank line as input means they are done editing)
        
        // User enters in shared password twice
        
        // Encrypt email with shared password
        
        // Add time stamp to email
        
        // "Send email" by storing it in the receiver's database entry
    }    
}

void delete_email(int email_number){
    // Check if email exists in database
    
    // Ask the user if they are sure
    
    // Delete email entry from database
}
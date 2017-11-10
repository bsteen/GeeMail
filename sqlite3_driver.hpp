/*
this is a class to use sqlite commands using cpp
*/

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

extern sqlite3 *db;

typedef struct{
    string val0;
    string val1;
    string val2;
    string val3;
    string val4;
    string val5;
    string val6;
    string val7;
}bucket8_t;

typedef struct{
    string receiver;
    string sender;
    string time_stamp;
    string message;
    int read;  //0 for false , 1 for true
    string password;
    string salt;
} Message_t;

typedef struct{
    string username;
    string password;
    string salt;
} User_t;

static int callback_view_all(void *ptr,int argc,char **argv,char **azColName);
static int callback(void *ptr, int argc, char **argv, char **azColName);
class sql_driver{
    public:
        sql_driver();
        //making databases
        void open_database(const char * db_name);
        void close_database();
        //making tables
        void make_users_table(string tablename,string c1,string c2,string c3);
        void make_email_table(string tableName,string receiver,string sender,string time_stamp,string message,string read,string password,string salt);
        
        //display tables (no filters, no data saved to program)
        void quick_table_view(string tableName);
        
        //might get rid of this one
        void populate_users_table(void *list,string tableName);
        
        //insert entries to tables
        void insert_user(string tableName,User_t newUser);
        void insert_email(string tableName,Message_t email);
        
        //Update table entries
        void update_entry(string tableName,string target_column,string condition_column,string condition,string new_val);
        
        //delete entry from tables
        void delete_user(string tableName,string username);
        void delete_entry(string tableName,string column_id,string condition);
        
        // search functions
        void get_all_usernames(string tableName,vector<bucket8_t> &list);
        void conditional_search(string tableName,string column_id,string contition,vector<bucket8_t> &bucket);
        void double_condition_search(string tableName,string column_id1,string condition1,string column_id2,string condition2,vector<bucket8_t> &bucket);
       
       //delete table completely
        void drop_table(string tableName);
    private:
        void execute_cmd(const char* sql_cmd,const char * debug_mssg,void * callback_data);
};
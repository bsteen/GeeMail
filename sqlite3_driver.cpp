#include "sqlite3_driver.hpp"
// #include <sqlite3.h>

sqlite3 *db;

static int callback(void *ptr, int argc, char **argv, char **azColName){
    // cout<<"inside the callback function very top"<<endl;
    vector<bucket8_t> *list = reinterpret_cast<vector<bucket8_t>*>(ptr);
    // cout<<"after casting the pointer"<<endl;
    bucket8_t b;
    
    b.val0 = argv[0]?argv[0]:"";
    if(argc>1){
        b.val1 = argv[1]?argv[1]:"";
    }
    if(argc>2){
        b.val1 = argv[1]?argv[1]:"";
    }
    if(argc>3){
        b.val2 = argv[2]?argv[2]:"";
    }
    if(argc>4){
        b.val3 = argv[3]?argv[3]:"";
    }
    if(argc>5){
        b.val4 = argv[4]?argv[4]:"";
    }
    if(argc>6){
        b.val5 = argv[5]?argv[5]:"";
    }
    if(argc>7){
        b.val6 = argv[6]?argv[6]:"";
    }
    if(argc>8){
        b.val7 = argv[7]?argv[7]:"";
    }
    list->push_back(b);
    return 0;

}

static int callback_view_all(void *ptr, int argc, char **argv, char **azColName){
    
    cout<<"--------------------------------------"<<endl;
    for (int i = 0; i<argc;i++){
        cout<<azColName[i]<<" : " <<argv[i]<<endl;
    }
    cout<<"---------------------------------------"<<endl;

    return 0;
} 

// CLASS STUFF============================
//PUBLIC FUNCIONS
sql_driver::sql_driver(){}

void sql_driver::open_database(const char * db_name){
    int rc;
    /*open database*/
    rc = sqlite3_open(db_name, &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Opened database successfully %d\n",rc);
    }
}

void sql_driver::close_database(){
    sqlite3_close(db);
}


//MAKE THE TABLES
void sql_driver::make_users_table(string tablename,string c1,string c2, string c3){ // need to check input to make sure it does not overflow
    char buffer[800];
    const char * tname = tablename.c_str();
    const char * col1 = c1.c_str();
    const char * col2 = c2.c_str();
    const char * col3 = c3.c_str();
    // cout<<tname<<endl;
    sprintf(buffer,
    "CREATE TABLE %s ("  \
    "%s TEXT NOT NULL UNIQUE," \
    "%s TEXT NOT NULL," \
    "%s TEXT );",
    tname,col1,col2,col3);
    char* sql_cmd;
    sql_cmd = buffer;
    //execute comand
    execute_cmd(sql_cmd,"Failed to create users table: %s\n",0);
}

void sql_driver::make_email_table(string tableName,string receiver,string sender,string time_stamp,string message,string read,string password,string salt){
    const char * tname = tableName.c_str();
    const char * rcvr = receiver.c_str();
    const char * sndr = sender.c_str();
    const char * t_s = time_stamp.c_str();
    const char * mssg = message.c_str();
    const char *  rd  = read.c_str();
    const char * pass = password.c_str();
    const char * slt = salt.c_str();
    char buffer[500];
    char * sql_cmd;
    sprintf(buffer,
    "CREATE TABLE %s ("\
    "%s TEXT NOT NULL,"\
    "%s TEXT NOT NULL,"\
    "%s TEXT NOT NULL UNIQUE,"\
    "%s TEXT NOT NULL,"\
    "%s INT NOT NULL,"\
    "%s TEXT NOT NULL,"\
    "%s INT NOT NULL);",
    tname,rcvr,sndr,t_s,mssg,rd,pass,slt);
    sql_cmd = buffer;
    execute_cmd(sql_cmd,"failed to create email Table: %s\n",0);
}


void sql_driver::quick_table_view(string tableName){
    const char * tname = tableName.c_str();
    char buffer [50];
    char * sql_cmd;
    sprintf(buffer,
    "SELECT * FROM %s ;",tname);
    sql_cmd = buffer;
    int rc;
    char *zErrMsg = 0;
    rc = sqlite3_exec(db,sql_cmd,callback_view_all,0,&zErrMsg);
    if (rc != SQLITE_OK){
        fprintf(stderr,"Failed to show table: %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout,"Operation done successfully\n");
    }
}


void sql_driver::insert_user(string tableName,User_t newUser){
    char buffer [512]; // Check bufffer size if there is stack smashing
    int rc;
    
    // const char* data = "Callback function called";
    const char* tname = tableName.c_str();
    const char* username = newUser.username.c_str();
    const char* password = newUser.password.c_str();
    const char* salt    = newUser.salt.c_str();
    int id = 0;
    
    char * sql_cmd;
          /* Create SQL statement */
    sprintf(buffer,
    "INSERT INTO %s (username,password,salt)"\
    "VALUES ('%s', '%s', '%s');",
    tname,username,password,salt);
    sql_cmd = buffer;
   /* Execute SQL statement */
   execute_cmd(sql_cmd,"Failed to insert new user: %s\n",0);
    
}

void sql_driver::insert_email(string tableName,Message_t email){
    const char * tname = tableName.c_str();
    const char * rcvr = email.receiver.c_str();
    const char * sndr = email.sender.c_str();
    const char * t_s = email.time_stamp.c_str();
    const char * mssg = email.message.c_str();
    // const char *  rd  = email.read.c_str();
    const char * pass = email.password.c_str();
    const char * slt = email.salt.c_str();
    char buffer[500];
    char * sql_cmd;
    sprintf(buffer,
    "INSERT INTO %s (receiver,sender,time_stamp,message,read,password,salt)"\
    "VALUES ('%s','%s','%s','%s','%d','%s','%s');",
    tname,rcvr,sndr,t_s,mssg,email.read,pass,slt);
    
    sql_cmd = buffer;
    execute_cmd(sql_cmd,"Failed to insert element to email table: %s\n",0);
    
}
// ++++++++++++++++++ Update fucntion++++++++++++++
void sql_driver::update_entry(string tableName,string target_column,string condition_column,string condition,string new_val){
    char buffer[300];
    const char * tname = tableName.c_str();
    const char * tcol = target_column.c_str();
    const char * ccol = condition_column.c_str();
    const char * con = condition.c_str();
    const char * val = new_val.c_str();
    char * sql_cmd;
    sprintf(buffer,
    "UPDATE %s "\
    "SET %s = '%s' "\
    "WHERE %s = '%s' ;",
    tname,tcol,val,ccol,con);
    sql_cmd = buffer;
    execute_cmd(sql_cmd,"Failed to update entry: %s\n",0);
}

//deleting stuff section-------------------------------------------
void sql_driver::delete_user(string tableName,string username){
    char buffer [200];
    int rc;
    const char * tname = tableName.c_str();
    const char * user = username.c_str();
    
    char * sql_cmd;
    
    sprintf(buffer,
    "DELETE FROM %s "\
    "WHERE username ='%s';"\
    ,tname,user);
    sql_cmd = buffer;
    // cout<<buffer<<endl;
    execute_cmd(sql_cmd,"deleting user failed: %s\n",0);
}

void sql_driver::delete_entry(string tableName,string column_id,string condition){
    char buffer[200];
    const char *tname = tableName.c_str();
    const char *col = column_id.c_str();
    const char *con = condition.c_str();
    char * sql_cmd;
    sprintf(buffer,
    "DELETE FROM %s "\
    "WHERE %s = '%s';",
    tname,col,con);
    sql_cmd = buffer;
    execute_cmd(sql_cmd,"failed to delete entry: %s\n",0);
}


//SEARCH FUNCTIONS ☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺☺
void sql_driver::get_all_usernames(string tableName,vector<bucket8_t> &list){
    char buffer[200];
    char * sql_cmd;
    const char * tname = tableName.c_str();
    sprintf(buffer,
    "SELECT DISTINCT username FROM %s ;",
    tname);
    sql_cmd = buffer;
    int rc;
    char *zErrMsg = 0;
      
    list.clear();
    execute_cmd(sql_cmd,"Failed to fetch users: %s \n",&list);
    
}

void sql_driver::conditional_search(string tableName,string column_id,string contition,vector<bucket8_t> &bucket){
    char buffer[200];
    const char *tname = tableName.c_str();
    const char *col  = column_id.c_str();
    const char*con = contition.c_str();
    char* sql_cmd;
    
    sprintf(buffer,
    "SELECT * FROM %s WHERE %s = '%s' ;",
    tname,col,con);
    
    sql_cmd = buffer;
    bucket.clear();
    
    execute_cmd(sql_cmd,"Failed to get results: %s\n",&bucket);
    
    
}

void sql_driver::double_condition_search(string tableName,string column_id1,string condition1,string column_id2,string condition2,vector<bucket8_t> &bucket){
    const char* tname = tableName.c_str();
    const char* col1 = column_id1.c_str();
    const char* con1 = condition1.c_str();
    const char* col2 = column_id2.c_str();
    const char* con2 = condition2.c_str();
    char buffer[500];
    char * sql_cmd;
    sprintf(buffer,
    "SELECT * FROM %s WHERE "\
    "%s = '%s' AND %s = '%s' ;",
    tname,col1,con1,col2,con2);
    sql_cmd = buffer;
    bucket.clear();
    execute_cmd(sql_cmd,"Failed to find double condtion: %s\n",&bucket);
    
}



//delete them tables $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void sql_driver::drop_table(string tableName){

    char buffer [50];
    // const char* data = "Callback function called";
    const char* tname = tableName.c_str();
    
    sprintf(buffer,"DROP TABLE IF EXISTS %s;",tname);
    char *sql_cmd;
    sql_cmd = buffer;
      /* Execute SQL statement */
    execute_cmd(sql_cmd,"falied to drop table: %s\n",0);

}

//PRIVATE FUNCIONS
void sql_driver::execute_cmd(const char* sql_cmd,const char * debug_mssg,void * callback_data){
    int rc;
    char *zErrMsg = 0;
    
    rc = sqlite3_exec(db,sql_cmd,callback,callback_data,&zErrMsg);
    
    if( rc != SQLITE_OK ) {
        fprintf(stderr, debug_mssg, zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}

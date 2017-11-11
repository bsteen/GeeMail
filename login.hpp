#include <string>
#include "encrypt.hpp"
#include "sqlite3_driver.hpp"
using namespace std;

struct login_package{
    string username;
    string password;
    bool correct_format = false;
};

login_package attempt_login(const char *db_name, string tableName);
void attempt_register(const char *db_name, string tableName);
login_package request_input();
bool valid_credentials(string username, string password, size_t bytes);
void lowercase_convert(string &word);
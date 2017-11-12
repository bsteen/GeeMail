#include <string>
#include "sqlite3_driver.hpp"
using namespace std;

void display_inbox_all(sql_driver &driver, string username);
void display_inbox_new(string username);
void read_email(int email_number);
bool valid_username(sql_driver &driver, string username);
void send_email(sql_driver &driver, string sender);
void delete_email(int email_number);
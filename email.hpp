#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <string>
#include "sqlite3_driver.hpp"
using namespace std;

void print_email_header(int message_num, Message_t message);
void display_inbox_all(sql_driver &driver, string username);
void display_inbox_new(sql_driver &driver, string username);
void read_email(sql_driver &driver, string username, int inbox, int email_number);
bool valid_username(sql_driver &driver, string &username);
void send_email(sql_driver &driver, string sender);
void delete_email(sql_driver &driver, string username, int inbox, int email_number);

#endif
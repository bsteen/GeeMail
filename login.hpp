#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

struct login_package{
    string username;
    string password;
    bool correct_format = false;
};

login_package request_input();
bool valid_credentials(string username, string password, size_t bytes);

#include <string>
using namespace std;

string encrypt_message(string raw_message, string raw_shared_password);
string decrypt_message(string encrypted_message, string raw_shared_password);
string encrypt_password(string raw_password, string salt);
string generate_salt();
string get_hex_string(string input);
char to_hex_char(char input);
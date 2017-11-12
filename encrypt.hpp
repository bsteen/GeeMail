#include <string>
using namespace std;

string encrypt_password(string raw_password, string salt);
string generate_salt();

string cipher_message(string data, string raw_shared_password);
string encrypt_message(string raw_message, string raw_shared_password);
string decrypt_message(string encrypted_message, string raw_shared_password);

char char_to_hex(char input);
string get_hex_string(string ascii_hex_string);

char hex_to_char(char input);
string get_ascii_string(string input);
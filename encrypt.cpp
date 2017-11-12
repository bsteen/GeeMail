#include <iostream>
#include <gcrypt.h>
#include <stdlib.h>
#include <stdio.h>
#include "encrypt.hpp"

//The size of the salt to be generated
#define SALT_BYTE_SIZE 32

// Helper function for get_hex_string
// Converts a single ASCII "hex" character into it true raw hex value
char char_to_hex(char input){
    switch(input){
        case 'a':
            return 10;
        case 'b':
            return 11;
        case 'c':
            return 12;
        case 'd':
            return 13;
        case 'e':
            return 14;
        case 'f':
            return 15;
        default:
            return input - 48;
    }
}

// Converts an ASCII "hex" string into a string of its true raw hex values
string get_hex_string(string ascii_hex_string){
    char hex_string_buffer[ascii_hex_string.length() / 2];
    unsigned char byte;
    unsigned char upper_value;
    unsigned char lower_value;
    
    int j = 0;
    for(int i = 0; i < ascii_hex_string.length(); i+=2){
        byte = 0;
        upper_value = char_to_hex(ascii_hex_string.at(i));
        lower_value = char_to_hex(ascii_hex_string.at(i + 1));
        byte |= (upper_value << 4);
        byte |= lower_value;
        
        hex_string_buffer[j] = byte;
        j++;
    }
    
    string raw_hex_string(hex_string_buffer, ascii_hex_string.length() / 2);
    return raw_hex_string;
}

// Helper function for get_ascii_string
// Converts the lower 4 bits of a char into its ASCII hex representation
char hex_to_char(char input){
    switch(input & 0xf){
        case 10:
            return 'a';
        case 11:
            return 'b';
        case 12:
            return 'c';
        case 13:
            return 'd';
        case 14:
            return 'e';
        case 15:
            return 'f';
        default:
            return input + 48;
    }
}

// Converts an hex value string into the ASCII representaion
string get_ascii_string(string input){
    string hex_string = "";
    
    for(int i = 0; i < input.length(); i++){
        hex_string += hex_to_char(input[i] >> 4 & 0xf);
        hex_string += hex_to_char(input[i] & 0xf);
    }
    
    return hex_string;
}

// Generates a random, 256 bit salt using /dev/urandom
// Returns the ASCII string representation of the hex values for the salt
string generate_salt(){
    char salt_char[SALT_BYTE_SIZE];
    
    FILE *file = fopen("/dev/urandom", "r");
    
    if(file == NULL){
        cout << "ERROR: could not read /dev/urandom" << endl;
        return "";
    }
    else{
        fread(salt_char, SALT_BYTE_SIZE, 1, file);
        fclose(file);
        string salt_string(salt_char, SALT_BYTE_SIZE);
        return get_ascii_string(salt_string);
    }
}   

// Returns the SHA256 hashed version of the salted password
// Salt should be 256 bits (use the generate_salt function) if password is to stored in database
// Returns the ASCII string representation of the hex values for the encrypted password
string encrypt_password(string raw_password, string salt){
    string raw_salt = raw_password + salt;
    char hash_buffer[32];
    
    gcry_md_hash_buffer(GCRY_MD_SHA256, hash_buffer, raw_salt.c_str(), raw_salt.length());

    // Clear the memory where the raw password was stored
    raw_password.clear();
    raw_salt.clear();
    salt.clear();
    
    string hashed(hash_buffer, 32);
    
    return get_ascii_string(hashed);
}

// Function that actual encrypts/decrypts the data using the SALSA20 encryption cipher
// Called by both encrypt_message and decrypt_message functions
string cipher_message(string data, string raw_shared_password){
    gcry_error_t gcryError;
    gcry_cipher_hd_t hd;
    
    char salsa_key[32];
    char init_vector[8];
    memcpy(salsa_key, encrypt_password(raw_shared_password, "").c_str(), 32);
    memcpy(init_vector, salsa_key, 8); // Use the first 8 bytes of the hashed password as the initilization vector
    
    gcryError = gcry_cipher_open(&hd, GCRY_CIPHER_SALSA20, GCRY_CIPHER_MODE_STREAM, 0);
    if(gcryError){
        printf("gcry_cipher_open failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        data.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    gcryError = gcry_cipher_setkey(hd, salsa_key, 32);
    if (gcryError){
        printf("gcry_cipher_setkey failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        data.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    gcryError = gcry_cipher_setiv(hd, init_vector, 8);
    if (gcryError){
        printf("gcry_cipher_setiv failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        data.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    size_t data_length = data.length();   // Don't care about the null character
    char encrypted_message_buffer[data_length];
    char message_buffer[data_length];
    memcpy(message_buffer, data.c_str(), data_length);

    gcryError = gcry_cipher_encrypt(hd, encrypted_message_buffer, data_length, message_buffer, data_length);
    if (gcryError){
        printf("gcry_cipher_decrypt failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        data.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        memset(encrypted_message_buffer, 0, data_length);
        memset(message_buffer, 0, data_length);
        data_length = 0;
        gcry_cipher_close(hd);
        return "";
    }
    
    string encrypted_message(encrypted_message_buffer, data_length);
    
    data.clear();
    raw_shared_password.clear();
    memset(salsa_key, 0, 32);
    memset(init_vector, 0, 8);
    memset(encrypted_message_buffer, 0, data_length);
    memset(message_buffer, 0, data_length);
    data_length = 0;
    gcry_cipher_close(hd);
    
    return encrypted_message;
}

// Encrypts a message using SALSA20 and the shared password as the key 256 bit key
// Returns the ASCII string representation of the hex values for the encrypted message
string encrypt_message(string raw_message, string raw_shared_password){
    string encrypted_message = get_ascii_string(cipher_message(raw_message, raw_shared_password));
    
    raw_message.clear();
    raw_shared_password.clear();
    
    return encrypted_message;
}

// Takes in the the ASCII string representation of the hex values for the encrypted message and the raw password
// It then converts the ASCII hex back into raw hex, decrypts the raw hex, and returns the plain text string
string decrypt_message(string ascii_encrypted_message, string raw_shared_password){
    
    // Must convert the ASCII hex representaion to the raw hex representation
    string plaintext_message = cipher_message(get_hex_string(ascii_encrypted_message), raw_shared_password);
    
    ascii_encrypted_message.clear();
    raw_shared_password.clear();
    
    return plaintext_message;
}
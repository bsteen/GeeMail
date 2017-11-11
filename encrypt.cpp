#include <iostream>
#include <gcrypt.h>
#include <stdlib.h>
#include <stdio.h>
#include "encrypt.hpp"

//The size of the salt to be generated
#define SALT_BYTE_SIZE 32

// Helper function for get_hex_string
// Converts the lower 4 bits of a char into its hex representation
char to_hex_char(char input){
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

// Just used to test the output of the encryption functions
// Converts an ASCII string into a string of its hex values
string get_hex_string(string input){
    string hex_string = "";
    
    for(int i = 0; i < input.length(); i++){
        hex_string += to_hex_char(input[i] >> 4 & 0xf);
        hex_string += to_hex_char(input[i] & 0xf);
    }
    
    return hex_string;
}

// Generates a random, 256 bit salt using /dev/urandom
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
        return salt_string;
    }
}   

// Returns the SHA256 hashed version of the salted password
// Salt should be 256 bits (use the generate_salt function) if password is to stored in database
string encrypt_password(string raw_password, string salt){
    string raw_salt = raw_password + salt;
    char hash_buffer[32];
    
    gcry_md_hash_buffer(GCRY_MD_SHA256, hash_buffer, raw_salt.c_str(), raw_salt.length());

    // Clear the memory where the raw password was stored
    raw_password.clear();
    raw_salt.clear();
    salt.clear();
    
    string hashed(hash_buffer, 32);
    
    return hashed;
}

// Encrypts a message using SALSA20 and the shared password as the key 256 bit key
string encrypt_message(string raw_message, string raw_shared_password){
    gcry_error_t gcryError;
    gcry_cipher_hd_t hd;
    
    char salsa_key[32];
    char init_vector[8];
    memcpy(salsa_key, encrypt_password(raw_shared_password, "").c_str(), 32);
    memcpy(init_vector, salsa_key, 8); // Use the first 8 bytes of the hashed password as the initilization vector
    
    gcryError = gcry_cipher_open(&hd, GCRY_CIPHER_SALSA20, GCRY_CIPHER_MODE_STREAM, 0);
    if(gcryError){
        printf("gcry_cipher_open failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        raw_message.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    gcryError = gcry_cipher_setkey(hd, salsa_key, 32);
    if (gcryError){
        printf("gcry_cipher_setkey failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        raw_message.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    gcryError = gcry_cipher_setiv(hd, init_vector, 8);
    if (gcryError){
        printf("gcry_cipher_setiv failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        raw_message.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        gcry_cipher_close(hd);
        return "";
    }
    
    size_t message_length = raw_message.length();   // Don't care about the null character
    char encrypted_message_buffer[message_length];
    char message_buffer[message_length];
    memcpy(message_buffer, raw_message.c_str(), message_length);

    gcryError = gcry_cipher_encrypt(hd, encrypted_message_buffer, message_length, message_buffer, message_length);
    if (gcryError){
        printf("gcry_cipher_decrypt failed:  %s/%s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));
        raw_message.clear();
        raw_shared_password.clear();
        memset(salsa_key, 0, 32);
        memset(init_vector, 0, 8);
        memset(encrypted_message_buffer, 0, message_length);
        memset(message_buffer, 0, message_length);
        message_length = 0;
        gcry_cipher_close(hd);
        return "";
    }
    
    string encrypted_message(encrypted_message_buffer, message_length);
    
    raw_message.clear();
    raw_shared_password.clear();
    memset(salsa_key, 0, 32);
    memset(init_vector, 0, 8);
    memset(encrypted_message_buffer, 0, message_length);
    memset(message_buffer, 0, message_length);
    message_length = 0;
    gcry_cipher_close(hd);
    
    return encrypted_message;
}

string decrypt_message(string encrypted_message, string raw_shared_password){
    string raw_message = encrypt_message(encrypted_message, raw_shared_password);
    encrypted_message.clear();
    raw_shared_password.clear();
    return raw_message;
}
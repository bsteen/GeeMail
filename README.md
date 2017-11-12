# Project 2: Gee-Mail. (Due date Nov. 13th)
## Created by: Casey Campbell, Miguel Hernandez, and Benjamin Steenkamer

Write a secure asynchronous (same-machine) message platform. Here is what is must do: When loaded prompt the user to sign-in or register.
When a user registers get their username and password. When a user logs in ask for username and validate password. If login fails, choose your response. Once logged in, tell them
the number of messages they have, list the users that have sent them a message. (You can limit this to one message from a given sender or add a timestamp or even a title to messages
if you want a level 2.) Then ask if they want to read messages or write messages. When writing messages prompt them for the receipient username, the message, and a shared passphrase
with that receipient. When they read a message let them select the message somehow, then prompt them for the shared passphrase with that receipient, then show a plaintext message.
IMPORTANT: do not ever store passwords, messages, or passphrases in plaintext, this is an encrypted messaging system. The rule sets in play for this stage are FIO and ERR.
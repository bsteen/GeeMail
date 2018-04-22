# Project 2: Gee-Mail (Due date Nov. 13th)
## Created by: Casey Campbell, Miguel Hernandez, and Benjamin Steenkamer
## CPEG 676 - Fall 2017, Dr. Novocin
<http://sec.prof.ninja/projects/>  
Write a secure\* asynchronous (same-machine) message platform. Here is what is must do: When loaded prompt the user to sign-in or register.
When a user registers get their username and password. When a user logs in ask for username and validate password. If login fails, choose your response. Once logged in, tell them
the number of messages they have, list the users that have sent them a message. (You can limit this to one message from a given sender or add a timestamp or even a title to messages
if you want a level 2.) Then ask if they want to read messages or write messages. When writing messages prompt them for the receipient username, the message, and a shared passphrase
with that receipient. When they read a message let them select the message somehow, then prompt them for the shared passphrase with that receipient, then show a plaintext message.
IMPORTANT: do not ever store passwords, messages, or passphrases in plaintext, this is an encrypted messaging system. The rule sets in play for this stage are FIO and ERR.  
  
**\*Developer's note**: After a code audit, several exploitable bugs have been found that can be used to gain access to an arbitrary user's account and delete their emails.
This program was created for academic purposes and should not be to store or send sensitive messages in any capacity.
I should further clarify that the intention of this project was to never actually achieve a truly secure system. Its purpose was to create a message system as best we could and then audit it as a "red team" exercise to find vulnerabilities, which we did.

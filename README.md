This is a project for my class.

The issue with this project is because I have to do it all in one single file.
That means no headers.h, headers.c, and no CMake.

All of this needs to be in main.c, which can be a huge issue.

My plan for this project is to make a program where the user 
can decide if they can use the default key for encryption/decryption,
or they can edit the default key or make a new file e.g. "key1.enc".
If a symbol doesn't exist in the user made key,
the program will pull the encrypted equivalent for the symbol 
from the "defaults.enc" file.

I am thinking of making the program use custom extension ".enc" or ".key" for the key files.
This file will also be used for documentation and note taking as I work on the project.

In the future - I might make a spinoff version or a V2 version
Where I'll use proper techniques and use my expanded expirience
from this project and what I learned in the meantime.

----------------------------------------------------------------------------

Update 25.12.2025.
    #Users will have an option to log in to use their saved keys that they have generated.#

The user is given an option to choose from 3 options.
1) Encrypt using a default key
2) Encrypt using a user generated key
3) Decrypt

A fourth option is to exit from the program for pretty purposes.

I will be making it so a user can make an account and log in and 
actually access their past made keys for encryption and decryption.

I am unsure if the default key should be hard coded or stored on a file.
Still deciding on that!!!
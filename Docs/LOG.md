Login screen ✅
Save user data in a file ✅
Generate a "unique" default user key ✅
Encryption with default key ❕
Decryption with default key
Let user generate their own key
Encryption with users key
Decryption with users key

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

----------------------------------------------------------------------------

Update 26.12.2025.

They default key will be diffrent for each user.
The key will take first 4 characters of the username and 4 characers of their password
and get first 8 bytes of the key, other 8 bytes will be padded.
This way - each user has their own SOMEWHAT unique key and the program is a bit safer.

ADDED 100+ LINES OF CODE AND ONLY ONE ERROR!!!!!!!!!!!!!!

Fixed sending of literal copy of struct witch lead to data loss
"Dynamic" generating key has been made.
I am done for today I will go and cry myself to sleep .
Its up for the future me to figure out encryption :3

----------------------------------------------------------------------------

Update 27.12.2025.

Now I need to figure out how to do encryption - I've done multiple seminars
on cryptography and even explaning how it's done - yet I have no idea how to do it practically.
Maybe other parts of the program will be enhanced over time as I am trying to figure out how to do encryption
and decryption.

I have decided on Vigenère style cipher

----------------------------------------------------------------------------

Update 28.12.2025.

After some tinkering I was able to get the encryption working (it was fast)
I found out that i can use modulo instead of creating a new variable
so instead of my previous logic
Enc[i] = (text[i] + KEY[j]) % 256;
if(KEY[j] != '\0'){
    code
}
else if(KEY[j] == '\0'){
    code
}

i can do it in one line !!!
Enc[i]= (text[i] + KEY[i % strlen(KEY)]) % 256;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>   // _getcwd
#include <limits.h>   // _MAX_PATH

#define MAX 30 //Max length of a users username andpassword separetly
#define KEY_LEN 16 //This key is 16 bytes long and will be hard coded into this program!!!

/////////////////////////////////Global variables/files that are actively used.//////////////////////////////////

FILE *fusr;
unsigned char PADDING_KEY[16+1] = {'l','o','r','e','m','i','p','s','u','m','r','i','g','b','y','a','\0'};

//User data
typedef struct User{
    unsigned char username[MAX+1];
    unsigned char password[MAX+1];
}User;

void Clear(){ //Clears the console
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

////////////////////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////

void Login(User *login); //Login screen for the user and login code.
void Create(User *login, unsigned char buf[(MAX*2)+1], unsigned char pom[(MAX*2)+1]); //User registration

void Check(User *login); //Checks users password.

void DefKey(User *login, unsigned char DEFAULT_KEY[KEY_LEN+1]); //Default key generation
void UsrKey(unsigned char USER_KEY[KEY_LEN+1]); //User key generation

//Encryption
void EncText(unsigned char DEFAULT_KEY[KEY_LEN+1]); //Case 1 - Text option
void EncFile(unsigned char DEFAULT_KEY[KEY_LEN+1]); //Case 1 - File option

//Decryption
int DChoice();
void Dec(unsigned char DEFAULT_KEY[KEY_LEN+1]);

//List of options that a user can choose from.
int Options(User *login);
int FileChoice();

//End of program.
void EOP(FILE* fusr, FILE* fbuf, unsigned char* buf1, unsigned char* buf2){
    if(fusr) fclose(fusr);
    if(fbuf) fclose(fbuf);
    if(buf1) free(buf1);
    if(buf2) free(buf2);
}

///////////////////////////////////////////////////////MAIN//////////////////////////////////////////////////////

int main(void){
    User login;
    unsigned char* DEFAULT_KEY; //This is where the default key will be stored. The key looks like User + Pass + Padding
    //This is extremely unsafe and I extremely advise against hard-coding default keys or any sensitive info into your program!!!
    unsigned char* USER_KEY;
    if(!(DEFAULT_KEY = malloc((KEY_LEN+1)*sizeof(char))) || !(USER_KEY = malloc((KEY_LEN+1)*sizeof(char)))){
        fprintf(stderr, "FATAL ERROR!\nCOULD NOT ALLOCATE MEMORY FOR KEYS!\nPRESS ENTER TO TERMINATE THE PROGRAM.");
        getchar();
        EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
    }

    Login(&login); //Login screen for the user.
    Clear();

    DefKey(&login, DEFAULT_KEY);
    Clear();

    while(1){
        Clear();
        switch (Options(&login)) {    //Options from which the user can choose from.
        case 1: //Encrypt with the default key
            Clear();
            switch(FileChoice()){ //User chooses what they want to encrypt and how.
                case 1: //Encrypt text
                    Clear();
                    EncText(DEFAULT_KEY);
                    break;
                case 2: //Encrypt file
                    Clear();
                    EncFile(DEFAULT_KEY);
                    break;
                case 3:
                    break;
                default: //ERROR
                    Clear();
                    fprintf(stderr,"Unknown error!");
                    EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
                    exit(1);
            }
            break;

        case 2: //Encrypt with user key
            Clear();
            UsrKey(USER_KEY);
            switch (FileChoice()){ //Text or file
                case 1: //Text
                    Clear();
                    EncText(USER_KEY);
                    break;
                case 2: //File
                    Clear();
                    EncFile(USER_KEY);
                    break;
                case 3:
                    break;
                default: //ERROR
                    Clear();
                    fprintf(stderr,"Unknown error!");
                    EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
                    exit(1);
                }
            break;
        case 3: //Decrypt
            Clear();
            switch (DChoice()){
                case 1: //Default key decryption
                    Clear();
                    Dec(DEFAULT_KEY);
                    break;
                case 2: //User key decryption
                    Clear();
                    UsrKey(USER_KEY);
                    Dec(USER_KEY);
                    break;
                default: //ERROR
                    Clear();
                    fprintf(stderr,"Unknown error!");
                    EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
                    exit(1);
            }
            break;

        case 4: //Exit program
            Clear();
            printf("Goodbye!");
            EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
            exit(0);

        default: //ERROR
            Clear();
            fprintf(stderr, "Unknown error!");
            EOP(fusr, NULL, DEFAULT_KEY, USER_KEY);
            exit(1);
        }
    }
    EOP(fusr, NULL, DEFAULT_KEY, USER_KEY); //End of program
    return 0;
}

////////////////////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////

void Login(User *login){ //Login screen for the user
    printf("Please enter your username: ");
    fgets(login->username, MAX+1, stdin);
    Check(login);
}

//Login info will be stored as: 
//username1,password1\n
//username2,password2\n
//...
void Check(User *login){  //Checks if the user exists and if the users.txt file exists
    unsigned char *buf, *pom;

    printf("Please enter your password: ");
    fgets(login->password, MAX+2, stdin);

    //This part of the code checks if '\n' exists
    if(strchr(login->username, '\n') != NULL){
        *strchr(login->username, '\n') = 0;
    }
    if(strchr(login->password, '\n') != NULL){
        *strchr(login->password, '\n') = 0;
    }

    //Memory allocation
    if(!(buf = malloc((2*MAX+2)*sizeof(char)))){
        fprintf(stderr, "Fatal error! Could not allocate memory! Line 184.\nPress enter to continue.");
        getchar();
        if(fusr != NULL)
            EOP(fusr, NULL, NULL, NULL);
        exit(1);
    }
    if(!(pom = malloc((2*MAX+2)*sizeof(char)))){
        fprintf(stderr, "Fatal error! Could not allocate memory! Line 190.\nPress enter to continue.");
        getchar();
        EOP(fusr, NULL, buf, NULL);
        exit(1);
    }

    memset(buf,0,(2*MAX+2)*sizeof(char));
    memset(pom,0,(2*MAX+2)*sizeof(char));

    //This part of the program checks if the file users.txt exists and if it does not it will generate it.
    if((fusr = fopen("users.txt", "r+")) == NULL){
        printf("users.txt does not exist.\nGenerating users.txt...\n");
        if((fusr = fopen("users.txt", "w+")) == NULL){
            fprintf(stderr, "Fatal error!\nTerminating the program. Press enter to continue.");
            getchar();
            EOP(fusr, NULL, buf, pom);
            exit(1);
        }
    }

    rewind(fusr); // Moves the cursor at the start of the file

    //I HAVE ACCIDENTALLY RECREATED STRCAT() !!!!
    // strncpy(buf, login.username, strlen(login.username)); //buf = "username-------------"
    // buf[strlen(login.username)] = ','; // buf = "username,------------"
    // strncpy(buf[strlen(login.username)+2], login.password, strlen(login.password)); //buf = "username,password"
    
    //This will just make it easier me to compare user info with stored info
    strcat(buf,login->username);
    strcat(buf, ",");
    strcat(buf, login->password);

    int chk = 1;
    while(fgets(pom, (2*MAX)+2, fusr) != NULL){ //Goes through the file to find coresponding creditentials
        if(strchr(pom, '\n') != NULL){
            *strchr(pom, '\n') = 0;
        }
        if(!strcmp(buf,pom)){ //Found corresponding creditentials
            chk = 0;
            break;
        }
    }
    if(chk == 1) // Did not find corresponding creditentials
        Create(login, buf, pom);
    EOP(NULL, NULL, buf, pom); //End of function
    return;
}

void Create (User *login, unsigned char buf[(MAX*2)+2], unsigned char pom[(MAX*2)+2]){  //Account creation
    Clear();
    printf("\nDo you want to create a new account?(y/n) ");
    char yn = getchar();
    if(yn == 'y'){
        fprintf(fusr,strcat(buf,"\n"));
        fflush(fusr);
    }
    else if(yn == 'n'){
        getchar();
        Login(login);
    }
    else{
        do{
            printf("Please enter a valid choise (y/n): ");
            yn = getchar();
            getchar();
        }while(yn != 'y' && yn != 'n');
    }
}

void DefKey(User *login, unsigned char DEFAULT_KEY[KEY_LEN+1]){  //Default key generation
    memset(DEFAULT_KEY,0,(KEY_LEN+1)*sizeof(char));
    strncat(DEFAULT_KEY,login->username,4);
    strncat(DEFAULT_KEY,login->password,4);
    strncat(DEFAULT_KEY,PADDING_KEY,16-strlen(DEFAULT_KEY));
}

void UsrKey(unsigned char USER_KEY[KEY_LEN+1]){  //User key generation
    Clear();
    memset(USER_KEY, 0, (KEY_LEN+1)*sizeof(char));
    do{
        printf("Please enter your key (4 - 16 characters): ");
        fgets(USER_KEY, KEY_LEN + 1, stdin);
        USER_KEY[KEY_LEN] = '\0';

        if(strchr(USER_KEY, '\n') != 0){ //Erases enter from the key
            *strchr(USER_KEY, '\n') = 0;
        }
        if(strlen(USER_KEY)<KEY_LEN){ //checks if the key is shorter than 16 and fills it in with the padding key
            strncat(USER_KEY, PADDING_KEY, 16-strlen(USER_KEY));
        }
    }while(strlen(USER_KEY)<4);

    USER_KEY[KEY_LEN] = '\0';
}

int Options(User *login){  //Main menu
    int n;
    printf("Welcome %s! \nPlease choose an option.\n", login->username);
    printf("1.\tEncrypt using a default key. (Quick encryption!)\n");
    printf("2.\tEncrypt using a user generated key. (Safer encryption!)\n");
    printf("3.\tDecrypt\n");
    printf("4.\tExit the program.\n");
    printf("Please type the number of the option: ");
    do{
        scanf("%d", &n);
        getchar();
        if(n<1 || n>4){
            printf("Incorrect option, try again!\n");
            printf("Please type the number of the option: ");
        }
    }while(n<1 || n>4);
    return n;
}

int FileChoice(){  //User chooses if they want to encrypt text or file
    int n;
    printf("1.\tEncrypt text\n2.\tEncrypt file\n3.\tReturn to main menu");
    printf("\nPlease type the number of the option: ");
    do{
        scanf("%d", &n);
        getchar();
        if(n<1 || n>3){
            printf("Incorrect option, please try again!\n");
            printf("Please type the number of the option: ");
        }
    }while(n<1 || n>3);
    return n;
}

void EncText(unsigned char KEY[KEY_LEN+1]){  //Default key text encryption
    unsigned char *Text;
    FILE *EncText;
    if((EncText = fopen("EncText.txt", "w")) == NULL){ //Opening file EncText.txt
        fprintf(stderr, "Could not open file EncText.txt! Terminating program.");
        EOP(fusr, NULL, NULL, NULL);
        exit(1);
    }
    if((Text = malloc((80+1)*sizeof(char))) == NULL){ //Memory allocation for Text
        fprintf(stderr, "Could not allocate memory for string Text! Terminating program.");
        EOP(fusr, EncText, NULL, NULL);
        exit(1);
    }
    printf("Please enter the text you want to encrypt (80 characters max!): ");
    fgets(Text, 80+1, stdin);
    if(strchr(Text, '\n') != NULL){ //Cleans the string from having enters.
        *strchr(Text, '\n') = 0;
    }

    //Encryption part!
    //int j = 0;
    unsigned char *TextEnc;
    if((TextEnc = malloc(strlen(Text)+1)) == NULL){
        fprintf(stderr, "Could not allocate memory for TextEnc string! Terminating program.\nPress enter to continue");
        getchar();
        EOP(fusr, EncText, Text, NULL);
        exit(1);
    }
    for(int i = 0; i<strlen(Text); i++){
        TextEnc[i] = (Text[i] + KEY[i % KEY_LEN]) % 256; //This is obfuscation not total encryption!
        // if(DEFAULT_KEY[j] != '\0'){
        //     j++;
        // }
        // else if(DEFAULT_KEY[j] == '\0'){
        //     j=0;
        // }
    }
    TextEnc[strlen(Text)] = '\0';
    rewind(EncText);
    fputs(TextEnc, EncText);
    fflush(EncText);

    char cwd[_MAX_PATH];
    if (_getcwd(cwd, _MAX_PATH) == NULL) {
        fprintf(stderr,"getcwd failed\nFile name is EncFile.txt and is located at the same location where the program is.");
        printf("\nPress enter to return to main menu.");
        getchar();

        EOP(NULL, EncText, Text, TextEnc);
        return;
    }
    printf("The encrypted text is: %s", TextEnc);
    printf("\nEnecrypted file is saved at:\n%s\\EncFile.txt\n", cwd);
    printf("\nPress enter to return to main menu.");
    getchar();

    EOP(NULL, EncText, Text, TextEnc);
    return;
}

void EncFile(unsigned char KEY[KEY_LEN+1]){  //Default key file encryption
    FILE *fraw, *fEnc;  //fraw is raw text, and fEnc is encrypted .txt file
    char *loc;  //Location of the file
    if(!(loc = malloc(151*sizeof(char)))){ //allocation of memory for file location
        fprintf(stderr, "Error allocating memory for loc! Terminating program.");
        EOP(fusr, NULL, NULL, NULL);
        exit(1);
    }
    printf("Please enter the location of the .txt file you want to encrypt (e.g. ../folder/file.txt): ");
    fgets(loc, 150, stdin);
    if(strchr(loc, '\n') != 0){ //Erases enter from the location
        *strchr(loc, '\n') = 0;
    }

    fraw = fopen(loc, "rb"); 
    do{
        if(fraw != NULL){
            break;
        }
        fprintf(stderr, "Could not open destination %s.\nEnter new destination; \nor write menu to return to the main menu or exit to exit the program: ", loc);
        fgets(loc, 150, stdin);
        if(strchr(loc, '\n') != 0){ //Erases enter from the location
            *strchr(loc, '\n') = 0;
        }
        if(strcmp(loc,"menu") == 0){
            EOP(NULL, NULL, loc, NULL);
            return;
        }
        else if (strcmp(loc,"exit") == 0){
            EOP(fusr, NULL, loc, NULL);
            exit(0);
        }
        fraw = fopen(loc, "rb");
    }while(fraw == NULL);

    fseek(fraw, 0 , SEEK_END); //Goes to the end of the file
    long len = ftell(fraw); //Say how many characters it has
    rewind(fraw); //Goes back to the start of the file.

    unsigned char *EncBuf, *RawBuf;     //Memory allocation for Raw contents and Encrypted encrypted.
    EncBuf = malloc(len * sizeof(char));
    RawBuf = malloc(len * sizeof(char));
    if(!EncBuf || !RawBuf){
        fprintf(stderr, "Memory allocation for encrypted and decrypted text failed!\nPress enter to continue");
        getchar();
        EOP(fusr, fraw, EncBuf, RawBuf); free(loc);
        exit(1);
    }

    //File encryption
    fread(RawBuf, 1, len, fraw); //Reads the contents of the file
    for(long i = 0; i<len; i++){
        EncBuf[i] = (RawBuf[i] + KEY[i % KEY_LEN]) % 256; //Obfuscating
    }

    //Inputs the encrypted file
    fEnc = fopen("EncFile.txt", "wb");
    rewind(fEnc);
    fwrite(EncBuf, 1, len, fEnc);
    fflush(fEnc);

    char cwd[_MAX_PATH];
    if (_getcwd(cwd, _MAX_PATH) == NULL) {
        fprintf(stderr,"getcwd failed\nFile name is EncFile.txt and is located at the same location where the program is.");
        printf("\nPress enter to return to main menu.");
        getchar();

        EOP(fraw, fEnc, EncBuf, RawBuf); free(loc);
        return;
    }
    printf("Enecrypted file is saved at:\n%s\\EncFile.txt\n", cwd);
    printf("\nPress enter to return to main menu.");
    getchar();

    EOP(fraw, fEnc, EncBuf, RawBuf); free(loc);
    return;
}

int DChoice(){  //Option between default key decryption or user key decryption
    int n;
    printf("What key do you want to use to decrypt a file?");
    printf("\n1.\tUsing default key.\n2.\tUsing user key.\n3.\tReturn to main menu");
    printf("\nChoose an option: ");
    do{
        scanf("%d", &n);
        getchar();
        if(n==1 || n==2 || n==3){
            break;
        }
    }while(n<1 || n>3);
    return n;
}

void Dec(unsigned char KEY[KEY_LEN+1]){  //Default key decryption
    FILE *fDec, *fEnc; //fDec is for decrypted and fEnc is for encrypted
    char *loc; //file location
    if(!(loc = malloc(151*sizeof(char)))){
        fprintf(stderr, "Error allocating memory for loc! Terminating program.");
        EOP(fusr, NULL, NULL, NULL);
        exit(1);
    }
    printf("Please enter the location of the encrypted file (e.g. ../folder/file.txt): ");
    fgets(loc, 150, stdin);
    if(strchr(loc, '\n') != 0){ //Erases enter from the location
        *strchr(loc, '\n') = 0;
    }

    fEnc = fopen(loc,"rb");
    do{
        if(fEnc != NULL){
            break;
        }
        fprintf(stderr, "Could not open destination %s.\nEnter new destination;\nor write menu to return to the main menu or exit to exit the program:  ", loc);
        fgets(loc, 150, stdin);
        if(strchr(loc, '\n') != 0){ //Erases enter from the location
            *strchr(loc, '\n') = 0;
        }
        if(strcmp(loc, "menu") == 0){
            free(loc);
            return;
        }
        else if(strcmp(loc, "exit") == 0){
            EOP(fusr, NULL, loc, NULL);
            exit(0);
        }
        fEnc = fopen(loc, "rb");
    }while(fEnc == NULL);

    fseek(fEnc, 0, SEEK_END);   //Puts the pointer at the end
    long len = ftell(fEnc);     //Gets the length of the string
    rewind(fEnc);       //Puts the pointer back at the start of the file

    unsigned char *EncBuf, *DecBuf;     //Memory allocation for Encrypted file and Decrypted file.
    EncBuf = malloc(len * sizeof(char));
    DecBuf = malloc(len * sizeof(char));
    if(!EncBuf || !DecBuf){
        fprintf(stderr, "Memory allocation for encrypted and decrypted text failed!\nPress enter to continue.");
        getchar();
        EOP(fusr, fEnc, EncBuf, DecBuf); free(loc);
        exit(1);
    }

    //Decryption
    fread(EncBuf, 1, len, fEnc); //Reading data
    for(long i = 0; i<len; i++){
        DecBuf[i] = (EncBuf[i] - KEY[i%KEY_LEN] + 256) % 256; //Deobfuscating data
    }

    fDec = fopen("DecryptedText.txt", "wb");
    if(!fDec){
        fprintf(stderr, "Could not open DecryptedText.txt\nPress enter to continue.");
        getchar();
        EOP(fusr, fEnc, EncBuf, DecBuf); free(loc);
        exit(1);
    }
    fwrite(DecBuf, 1, len, fDec);

    char cwd[_MAX_PATH];
    if (_getcwd(cwd, _MAX_PATH) == NULL) {
        fprintf(stderr,"getcwd failed\nFile name is DecryptedText.txt and is located at the same location where the program is.");
        printf("\nPress enter to return to main menu.");
        getchar();

        EOP(fEnc, fDec, EncBuf, DecBuf); free(loc);
        return;
    }
    EOP(fEnc, fDec, EncBuf, DecBuf); free(loc);

    printf("Decrypted file is saved at:\n%s\\DecryptedText.txt\n", cwd);
    printf("\nPress enter to return to main menu.");
    getchar();

    return;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30 //Max length of a users username andpassword separetly
#define KEY_LEN 16 //This key is 16 bytes long and will be hard coded into this program!!!
//This is extremely unsafe and I extremely advise against hardcoding default keys or any sensitive info into your program!!!

/////////////////////////////////////////////////Global variables/files that are actively used./////////////////////////////////////////////////

FILE *fkey;
FILE *fusr;
unsigned char DEFAULT_KEY[KEY_LEN+1]; //This is where the default key will be stored. The key looks like User + Pass + Padding
unsigned char USER_KEY[KEY_LEN+1];
unsigned char PADDING_KEY[8+1] = {'l','o','r','e','m','i','p','s','\0'};

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

/////////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////

void Login(User *login); //Login screen for the user and login code.
void Create(User *login, unsigned char buf[(MAX*2)+1], unsigned char pom[(MAX*2)+1]); //User registration

void Check(User *login); //Checks users password.

void DefKey(User *login); //Default key generation

//Main case options:
//Case 1 "Encrypt with default key"
    void DEncText(); //Case 1 - Text option
    void DEncFile(); //Case 1 - File option

//Case 2 "Encrypt with user key"

//Case 3 "Decrypt"
int DChoice();
    //Case 3 - Default
    void DDec();
    //Case 3 - User
    //void UDec();

//List of options that a user can choose from.
int Options(User *login);
int FileChoice();

//End of program.
void EOP(FILE* fake, FILE* fusr){
    fclose(fake);
    fclose(fusr);
}

/////////////////////////////////////////////////MAIN/////////////////////////////////////////////////

int main(void){
    User login;

    Login(&login); //Login screen for the user.
    Clear();

    DefKey(&login);
    Clear();

    while(1){
        Clear();
        switch (Options(&login)) {    //Options from which the user can choose from.
        case 1: //Encrypt with the default key
            Clear();
            switch(FileChoice()){ //User chooses what they want to encrypt and how.
                case 1: //Encrypt text
                    Clear();
                    DEncText();
                    break;
                case 2: //Encrypt file
                    Clear();
                    DEncFile();
                    break;
                default: //ERROR
                    Clear();
                    fprintf(stderr,"Unknown error!");
                    EOP(fkey, fusr);
                    exit(1);
            }
            break;

        case 2: //Encrypt with user key
            Clear();
            break;

        case 3: //Decrypt
            Clear();
            switch (DChoice()){
                case 1: //Default key decryption
                    Clear();
                    DDec();
                    break;
                case 2: //User key decryption
                    //UDec();
                    break;
                default: //ERROR
                    Clear();
                    fprintf(stderr,"Unknown error!");
                    EOP(fkey, fusr);
                    exit(1);
            }
            break;

        case 4: //Exit program
            Clear();
            printf("Goodbye!");
            EOP(fkey, fusr);
            exit(0);

        default: //ERROR
            Clear();
            fprintf(stderr, "Unknown error!");
            EOP(fkey, fusr);
            exit(1);
        }
    }
    EOP(fkey,fusr); //End of program
    return 0;
}

/////////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////  

void Login(User *login){
    printf("Please enter your username: ");
    fgets(login->username, MAX+1, stdin);
    Check(login);
}

//Login info will be stored as: 
//username,password\n
//username,password\n
//...
void Check(User *login){
    unsigned char *buf, *pom;

    if(!(buf = malloc((2*MAX+2)*sizeof(char)))){
        fprintf(stderr, "Fatal error! Could not allocate memory! Line 123.");
        EOP(fkey,fusr);
        exit(1);
    }
    if(!(pom = malloc((2*MAX+2)*sizeof(char)))){
        fprintf(stderr, "Fatal error! Could not allocate memory! Line 123.");
        free(buf);
        EOP(fkey,fusr);
        exit(1);
    }

    memset(buf,0,(2*MAX+2)*sizeof(char));
    memset(pom,0,(2*MAX+2)*sizeof(char));

    //This part of the program checks if the file users.txt exists and if it does not it will generate it.
    if((fusr = fopen("users.txt", "r+")) == NULL){
        fclose(fusr);
        printf("users.txt does not exist.\nGenerating users.txt...\n");
        if((fusr = fopen("users.txt", "w+")) == NULL){
            fprintf(stderr, "Fatal error!\nTerminating the program.");
            EOP(fkey, fusr);
            free(buf);
            free(pom);
            exit(1);
        }
    }

    printf("Please enter your password: ");

    fgets(login->password, MAX+2, stdin);

    //This part of the code checks if '\n' exists
    if(strchr(login->username, '\n') != NULL){
        *strchr(login->username, '\n') = 0;
    }
    if(strchr(login->password, '\n') != NULL){
        *strchr(login->password, '\n') = 0;
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
    free(buf);
    free(pom);
}

void Create (User *login, unsigned char buf[(MAX*2)+2], unsigned char pom[(MAX*2)+2]){
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
        }while(yn != 'y' || yn != 'n');
    }
}

void DefKey(User *login){
    // char baf[30];
    //buf = malloc (5*sizeof(char));
    strncat(DEFAULT_KEY,login->username,4);
    strncat(DEFAULT_KEY,login->password,4);
    strncat(DEFAULT_KEY,PADDING_KEY,16-strlen(DEFAULT_KEY));
}

int Options(User *login){
    int n;
    printf("Welcome %s! \nPlease choose an option.\n", login->username);
    printf("1.\tEncrypt using a default key.\n");
    printf("2.\tEncrypt using a user generated key. (does not work)\n");
    printf("3.\tDecrypt (Work in progress.)\n");
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

int FileChoice(){
    int n;
    printf("1.\tEncrypt text\n2.\tEncrypt file");
    printf("\nPlease type the number of the option: ");
    do{
        scanf("%d", &n);
        getchar();
        if(n<1 || n>2){
            printf("Incorrect opetion, try again!\n");
            printf("Please type the number of the option: ");
        }
    }while(n<1 || n>2);
    return n;
}

void DEncText(){
    unsigned char *Text;
    FILE *EncText;
    if((EncText = fopen("EncText.txt", "w")) == NULL){ //Opening file EncText.txt
        fprintf(stderr, "Could not open file EncText.txt! Terminating program.");
        EOP(fkey, fusr);
        exit(1);
    }
    if((Text = malloc((80+1)*sizeof(char))) == NULL){ //Memory allocation for Text
        fprintf(stderr, "Could not allocate memory for string Text! Terminating program.");
        EOP(fkey, fusr);
        fclose(EncText);
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
        fprintf(stderr, "Could not allocate memory for TextEnc string! Terminating program.");
        free(Text);
        fclose(EncText);
        EOP(fkey, fusr);
        exit(1);
    }
    for(int i = 0; i<strlen(Text); i++){
        TextEnc[i] = (Text[i] + DEFAULT_KEY[i % KEY_LEN]) % 256;
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
    free(Text);
    free(TextEnc);
}

void DEncFile(){  //Encrypts an existing .txt file!
    FILE *fraw, *fEnc;  //fraw is raw text, and fEnc is encrypted .txt file
    char *loc;  //Location of the file
    if(!(loc = malloc(151*sizeof(char)))){ //allocation of memory for file location
        fprintf(stderr, "Error allocating memory for loc! Terminating program.");
        EOP(fkey, fusr);
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
        fclose(fraw);
        fprintf(stderr, "Could not open destination %s.\nEnter new destination: ", loc);
        fgets(loc, 150, stdin);
        if(strchr(loc, '\n') != 0){ //Erases enter from the location
            *strchr(loc, '\n') = 0;
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
        fprintf(stderr, "Memory allocation for encrypted and decrypted text failed!");
        free(loc);
        free(EncBuf);
        free(RawBuf);
        fclose(fraw);
        EOP(fkey,fusr);
        exit(1);
    }

    //File encryption

    fread(RawBuf, 1, len, fraw); //Reads the contents of the file :3

    for(long i = 0; i<len; i++){
        EncBuf[i] = (RawBuf[i] + DEFAULT_KEY[i % KEY_LEN]) % 256;
    }

    //Inputs the encrypted file
    fEnc = fopen("EncFile.txt", "wb");
    rewind(fEnc);
    fwrite(EncBuf, 1, len, fEnc);
    fflush(fEnc);

    free(loc);
    fclose(fraw);
    fclose(fEnc);
}

int DChoice(){
    int n;
    printf("How do you want to decrypt a file?");
    printf("\n1.\tUsing default key.\n2.\tUsing user key.");
    printf("Choose an option: ");
    do{
        scanf("%d", &n);
        getchar();
        if(n==1 || n==2){
            break;
        }
    }while(n<1 || n>2);
    return n;
}

void DDec(){
    FILE *fDec, *fEnc; //fDec is for decrypted and fEnc is for encrypted
    char *loc; //file location
    if(!(loc = malloc(151*sizeof(char)))){
        fprintf(stderr, "Error allocating memory for loc! Terminating program.");
        EOP(fkey,fusr);
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
        fclose(fEnc);
        fprintf(stderr, "Could not open destination %s.\nEnter new destination: ", loc);
        fgets(loc, 150, stdin);
        if(strchr(loc, '\n') != 0){ //Erases enter from the location
            *strchr(loc, '\n') = 0;
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
        fprintf(stderr, "Memory allocation for encrypted and decrypted text failed!");
        free(loc);
        free(EncBuf);
        free(DecBuf);
        fclose(fEnc);
        EOP(fkey,fusr);
        exit(1);
    }

    //Decryption from the file.

    fread(EncBuf, 1, len, fEnc);

    for(long i = 0; i<len; i++){
        DecBuf[i] = (EncBuf[i] - DEFAULT_KEY[i%KEY_LEN] + 256) % 256;
    }

    fDec = fopen("DecryptedText.txt", "wb");
    if(!fDec){
        fprintf(stderr, "Could not open DecryptedText.txt");
        free(EncBuf);
        free(DecBuf);
        fclose(fEnc);
        EOP(fkey,fusr);
        exit(1);
    }
    fwrite(DecBuf, 1, len, fDec);
    // fputs(DecBuf, fDec);

    fclose(fEnc);
    fclose(fDec);
    free(loc);
    free(EncBuf);
    free(DecBuf);
}
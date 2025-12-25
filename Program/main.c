#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 30 //Max length of a users username andpassword separetly
#define KEY_LEN 16 //This key is 16 bytes long and will be hard coded into this program!!!
//This is extremely unsafe and I extremely advise against hardcoding default keys or any sensitive info into your program!!!

//Global variables/files that are actively used.
FILE *fake;
FILE *fusr;
unsigned char DEFAULT_KEY[KEY_LEN] = {
    'D','E','F','A','U','L','T','_','K','E','Y','_','1','2','3','4' //This is what a default key will look like until i think of something better...
};

//This struct has a password and username of a user
typedef struct User{
    char username[MAX+1];
    char password[MAX+1];
}user;

void Clear(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//Login screen for the user and login code.
void Login(user login);
//Checks users password.
void Check(user login);

//End of program.
void EOP(FILE* fake, FILE* fusr){
    fclose(fake);
    fclose(fusr);
}

//Encryption function
void DefEnc();
//Where the key is the user generated key pulled out from the .key
void UsrEnc();

//List of options that a user can choose from.
int Options();
int FileChoice();

int main(void){
    user login;

    Login(login); //Login screen for the user.

    Clear();
    switch (Options()) {    //Options from which the user can choose from.
    case 1: //Encrypt with the default key
        Clear();
        switch(FileChoice()){ //User chooses what they want to encrypt and how.
            case 1: //Encrypt text
                Clear();
                break;
            case 2: //Encrypt file
                Clear();
                break;
            default: //ERROR
                Clear();
                fprintf(stderr,"Unknown error!");
                EOP(fake, fusr);
                exit(1);
        }
        break;
    case 2: //Encrypt with user key
        Clear();
        break;
    case 3: //Decrypt
        Clear();
        break;
    case 4: //Exit program
        Clear();
        printf("Goodbye!");
        EOP(fake, fusr);
        exit(0);
    default: //ERROR
        Clear();
        fprintf(stderr, "Unknown error!");
        EOP(fake, fusr);
        exit(1);
    }

    EOP(fake,fusr); //End of program
    return 0;
}
void Login(user login){
    printf("Please enter your username: ");
    fgets(login.username, MAX+1, stdin);
    Check(login);
}

void Check(user login){
    //This part of the program checks if the file users.txt exists and if it does not it will generate it.
    if((fake = fopen("users.txt", "r+")) == NULL){
        printf("users.txt does not exist.\nGenerating users.txt...\n");
        if((fusr = fopen("users.txt", "w+")) == NULL){
            fprintf(stderr, "Fatal error!\nTerminating the program.");
            EOP(fake, fusr);
        }
    }
}

int Options(){
    int n;
    printf("Welcome!\nPlease choose an option.\n");
    printf("1.\tEncrypt using a default key. (work in progress)\n");
    printf("2.\tEncrypt using a user generated key. (does not work)\n");
    printf("3.\tDecrypt (does not work)\n");
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
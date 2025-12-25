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
    'D','E','F','A','U','L','T','_','K','E','Y','_','1','2','3','4' //This is what a default key will look like until i think of somethig better...
};

//This struct has a password and username of a user
typedef struct User{
    char username[MAX+1];
    char password[MAX+1];
}user;

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

int main(void){
    user login;

    //Login screen for the user.
    Login(login);

    //Options from which the user can choose from.
    switch (Options())
    {
    case 1: //Encrypt with the default key
        break;
    case 2: //Encrypt with user key
        break;
    case 3: //Decrypt
        break;
    case 4: //Exit program
        printf("Goodbye!");
        EOP(fake, fusr);
        exit(0);
    default:
        printf("Unknown error!");
        exit(1);
    }

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
        printf("Generating users.txt...\n");
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
            printf("Incorrect option try again!\n");
            printf("Please type the number of the option: ");
        }
    }while(n<1 || n>4);
    return n;
}
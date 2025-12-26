#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/md5.h>

#define MAX 30 //Max length of a users username andpassword separetly
#define KEY_LEN 16 //This key is 16 bytes long and will be hard coded into this program!!!
//This is extremely unsafe and I extremely advise against hardcoding default keys or any sensitive info into your program!!!

//Global variables/files that are actively used.
FILE *fake;
FILE *fusr;
unsigned char DEFAULT_KEY[KEY_LEN]; //This is where the key will be stored and it will steal the users password.

//This struct has a password and username of a user
typedef struct User{
    char username[MAX+1];
    char password[MAX+1];
}User;

void Clear(){ //Clears the console
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/////////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////

//Login screen for the user and login code.
void Login(User login);
void Create(User login, char buf[(MAX*2)+1], char pom[(MAX*2)+1]); //User registration
//Checks users password.
void Check(User login);


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

/////////////////////////////////////////////////MAIN/////////////////////////////////////////////////

int main(void){
    User login;

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

/////////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////  

void Login(User login){
    printf("Please enter your username: ");
    fgets(login.username, MAX+1, stdin);
    Check(login);
}

//Login info will be stored as: 
//username,password\n
//username,password\n
//...

void Check(User login){
    char buf[(2*MAX)+1], pom[(2*MAX)+1];
    memset(buf,0,sizeof(buf));
    memset(pom,0,sizeof(pom));

    //This part of the program checks if the file users.txt exists and if it does not it will generate it.
    if((fusr = fopen("users.txt", "r+")) == NULL){
        fclose(fusr);
        printf("users.txt does not exist.\nGenerating users.txt...\n");
        if((fusr = fopen("users.txt", "w+")) == NULL){
            fprintf(stderr, "Fatal error!\nTerminating the program.");
            EOP(fake, fusr);
            exit(1);
        }
    }

    printf("Please enter your password: ");

    fgets(login.password, MAX+1, stdin);

    //This part of the code checks if '\n' exists
    if(strchr(login.username, '\n') != NULL){
        *strchr(login.username, '\n') = 0;
    }
    if(strchr(login.password, '\n') != NULL){
        *strchr(login.password, '\n') = 0;
    }

    rewind(fusr);

    //I HAVE ACCIDENTALLY RECREATED STRCAT() !!!!
    // strncpy(buf, login.username, strlen(login.username)); //buf = "username-------------"
    // buf[strlen(login.username)] = ','; // buf = "username,------------"
    // strncpy(buf[strlen(login.username)+2], login.password, strlen(login.password)); //buf = "username,password"
    
    //This will just make it easier me to compare user info with stored info
    strcat(buf,login.username);
    strcat(buf, ",");
    strcat(buf, login.password);

    int chk = 1;
    while(fgets(pom, (2*MAX)+1, fusr) != NULL){
        if(strchr(pom, '\n') != NULL){
            *strchr(pom, '\n') = 0;
        }
        if(!strcmp(buf,pom)){
            chk = 0;
            break;
        }
    }
    if(chk == 1)
        Create(login, buf, pom);
}

void Create (User login, char buf[(MAX*2)+1], char pom[(MAX*2)+1]){
    Clear();
    printf("\nDo you want to create a new account?(y/n) ");
    char yn = getchar();
    if(yn == 'y'){
        //code
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
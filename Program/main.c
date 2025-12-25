#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 30 //Max length of a users username andpassword separetly
#define KEY "HelloWorld123456" //This key is 16 bytes long and will be hard coded into this program!!!
//This is extremely unsafe and I extremely advise against hardcoding default keys or any sensitive info into your program!!!

//This struct has a password and username of a user
typedef struct User{
    char username[MAX+1];
    char password[MAX+1];
}user;

//Login screen for the user and login code.
void Login(char Username[MAX+1]);
//Checks users password.
void Check(char Username[MAX+1]);

//List of options that a user can choose from.
int Options();

int main(void){
    FILE *fkey;


    //Options from which the user can choose from.
    switch (Options())
    {
    case 1:
        /* code */
        break;
    case 2:
        //
        break;
    case 3:
        //
        break;
    case 4:
        printf("Goodbye!");
        exit(0);
    default:
        printf("Unknown error!");
        exit(1);
    }

    fclose(fkey);
    return 0;
}
void Login(char Username[MAX+1]){
    pritnf("Please inout your username: ");
    Check(Username);
}
int Options(){
    int n;
    printf("Welcome!\n",
    "Please choose an option.");
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
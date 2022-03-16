#include<string.h>

bool isnum(char* d_name){
    for(int i = 0 ; d_name[i] != '\0' ; i++){
        if(d_name[i] < '0' || d_name[i] > '9') return false;
    }

    return true;
}

char* get_uid(const char *dir){
    char *status_path = new char[30];
    strcpy(status_path, dir);
    strcat(status_path, "/status");
    FILE *status_file = Fopen(status_path, "r");
    char *uid_line = new char[1000];
    for(int i = 1 ; i <= 9 ; i++)
        fgets(uid_line, 100, status_file);
    char *uid = new char[10];
    sscanf(uid_line, "Uid: %s", uid);    
    return uid;
}
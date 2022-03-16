bool isnum(char* d_name){
    for(int i = 0 ; d_name[i] != '\0' ; i++){
        if(d_name[i] < '0' || d_name[i] > '9') return false;
    }

    return true;
}
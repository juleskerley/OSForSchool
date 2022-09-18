void k_clearscr();
void k_printstr(char *string, int row, int col);



int main(){
    while(1){
    char *string = "hello world";
    k_printstr(string, 0, 0);
    }
    return 0;
}

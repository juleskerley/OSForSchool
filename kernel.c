void k_clearscr();
void k_printstr(char *string, int row, int col);



int main(){
    char *help = "hello world";
    k_printstr(*help, 10, 10);
    return 0;
}

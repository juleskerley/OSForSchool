void k_clearscr();
void k_printstr(char *string, int row, int col);
void run_test();


int main(){
    char *string = "hello world";
    k_printstr(string, 0, 0);
    run_test();
    while(1){} // This keeps the screen from flickering
    return 0;
}

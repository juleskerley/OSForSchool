void k_clearscr();
void k_printstr(char *string, int row, int col);



int main(){
    const char *string = "hello world";
    k_printstr(string, 11, 17);
    return 0;
}

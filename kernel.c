void k_printstr(char *string, int row, int col);
void k_clearscr();
void run_test();
void print_border(int start_row, int start_col, int end_row, int end_col);


int main(){
    k_clearscr();
    print_border(0,0,25,80);
    run_test();
    while(1){} // This keeps the screen from flickering
    return 0;
}

void k_clearscr(){
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 80; j++)
            k_printstr(" ", i, j);
}

void print_border(int start_row, int start_col, int end_row, int end_col){
    for (int i = 0; i < 25; i++){
        for (int j = 0; j < 80; j++){
            if (i == 0 && j == 0 || i == 24 && j == 0 || i == 0 && j == 79 || i == 24 && j == 79)
                k_printstr("+", i, j);
            else if (i == 0 || i == 24)
                k_printstr("-", i, j);
            else if (j == 0 || j == 79)
                k_printstr("|", i, j);
        }
    }
}




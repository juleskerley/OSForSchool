void k_printstr(char *string, int row, int col);
void k_clearscr();
void run_test();
void print_border(int start_row, int start_col, int end_row, int end_col);


int main(){
    k_clearscr();
    // TODO
    k_printstr(string, 0, 0);
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



}




#include <sddint.h>

void run_test();

void k_printstr(char *string, int row, int col);
void k_clearscr();
void print_border(int start_row, int start_col, int end_row, int end_col);

void enqueue(pcbq_t *q, pcb_t *pcb);
pcb_t *dequeue(pcbq_t *q);

int create_process(uint32_t code_address);

/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

int main(){
    k_clearscr();
    k_printstr("Running Processes", 0, 0);
    while(1){} // This keeps the screen from flickering
    return 0;
}

void k_clearscr(){
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 80; j++)
            k_printstr(" ", i, j);
}

void print_border(int start_row, int start_col, int end_row, int end_col){
    // Required for the usage; also prevents end + from being overwritten
    end_row--;
    end_col--;

    k_printstr("+", start_row, start_col);
    k_printstr("+", start_row, end_col);
    k_printstr("+", end_row, start_col);
    k_printstr("+", end_row, end_col);

    for (int i = start_row+1; i < end_row; i++){
        k_printstr("|", i, start_col);
        k_printstr("|", i, end_col);
    }
    for (int i = start_col+1; i < end_col; i++){
        k_printstr("-", start_row, i);
        k_printstr("-", end_row, i);
    }
}




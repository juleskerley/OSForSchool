/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

#include <sddint.h>

// Dr. Roger's code
void run_test();
void buddy_init();

// prototypes
void k_printstr(char *string, int row, int col);
void k_clearscr();
void print_border(int start_row, int start_col, int end_row, int end_col);


void enqueue(pcbq_t *q, pcb_t *pcb);
pcb_t *dequeue(pcbq_t *q);

int create_process(uint32_t code_address);

// structs

// Node; process control block
struct pcb_t {

}
// Queue
struct pcbq_t {
}

int main(){
    buddy_init();
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
    // Required for the usage: Makes the values 'human-readable' (1 indexed)
    // also prevents end + from being overwritten
    end_row--;
    end_col--;
    start_row--;
    start_col--;

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


int create_process(uint32_t code_address){
    uint32_t *stackptr = k_malloc(1024);

    uint32_t *st = stackptr+1024;

    *st--; // Moving the mem location down 1
    uint32_t eflags = 0;
    st = eflags; // Setting current mem location to eflags (0)

    *st--;
    uint32_t cs;
    st = cs;

    st--;
    st = code_address;

    st--;
    uint32_t dispatch_leave;
    st = dispatch_leave;

}

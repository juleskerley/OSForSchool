/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

#include <stdint.h>

// global variables
uint32_t pid=0;

// structs

// Node; process control block
struct pcb_t {
    // Data

    struct pcb_t *next;
};
// Queue for above node
struct pcbq_t {
    struct pcb_t *start;
    struct pcb_t *rear;


};

// Dr. Roger's code
void run_test();
void buddy_init();

// prototypes
void k_printstr(char *string, int row, int col);
void k_clearscr();
void print_border(int start_row, int start_col, int end_row, int end_col);


void enqueue(struct pcbq_t *q, struct pcb_t *pcb);
struct pcb_t *dequeue(struct pcbq_t *q);

int create_process(uint32_t code_address);


int main(){
    buddy_init();
    k_clearscr();
    print_border(0,0,24,79);
    k_printstr("Running Processes", 1, 1);
    while(1){} // This keeps the screen from flickering
    return 0;
}

void k_clearscr(){
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 80; j++)
            k_printstr(" ", i, j);
}

void print_border(int start_row, int start_col, int end_row, int end_col){
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
    uint32_t *stackptr = kmalloc(1024);
    if (stackptr == nullptr)
        return -1;

    uint32_t *st = stackptr+1024;

    *st--; // Moving the mem location down 1
    uint32_t eflags = 0;
    st = eflags; // Setting current mem location to eflags (0)

    *st--;
    uint32_t cs = 0;
    st = cs;

    *st--;
    uint32_t code_address = 0;
    st = cs;

    *st--;
    uint32_t dispatch_leave = 0;
    st = dispatch_leave;

    for (int i = 0; i < 8; i++){
        *st--;
        st = 0;
    }


    return 0;
}

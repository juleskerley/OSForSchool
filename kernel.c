/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

#include <stdint.h>

// global variables
uint32_t pid=0;

// Dr. Roger's code
void run_test();
void buddy_init();

// queue structs
// Node; process control block
typedef struct {
    // Data
    uint32_t pid;
    uint32_t esp;
    pcb_t *next;
    pcb_t *prev;
} pcb_t;
// Queue for above node
typedef struct {
    pcb_t *start;
    pcb_t *end;
    
} pcbq_t;

// queue prototypes
void enqueue(pcbq_t *q, pcb_t *pcb);
pcb_t *dequeue(pcbq_t *q);

// text prototypes
void k_printstr(char *string, int row, int col);
void k_clearscr();
void print_border(int start_row, int start_col, int end_row, int end_col);


// process prototypes
int create_process(uint32_t code_address);
void p1();
void p2(); //I'm going to chop myself up in the meat grinder if I miss another semicolon

int main(){
    buddy_init();
    k_clearscr();
    print_border(0,0,24,79);
    k_printstr("Running Processes", 1, 1);
    p1();
    p2();
    create_process(0);
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
    uint32_t *stackptr = kmalloc(1024*sizeof(uint32_t));

    if (stackptr == 0)
        return -1;


    uint32_t *st = stackptr+1024; // I want st to be a stored pointer of the malloc

    st--; // Moving the mem location down 1
    uint32_t eflags = 0;
    *st = eflags; // Setting current mem location to eflags (0)

    st--;
    uint32_t cs = 0;
    *st = cs;

    st--;
    *st = code_address;
    st--;
    uint32_t dispatch_leave = 0;
    *st = dispatch_leave;

    for (int i = 0; i < 8; i++){
        st--;
        *st = 0;
    }


    return 0;
}

void p1(){
    print_border(10, 10, 13, 35);
    k_printstr("Process 1 running...", 11, 11);
    uint32_t num = 0;
    k_printstr("value: ", 12, 11);
    while (1){
    	char numStr[5] = "";
        convert_num(num, numStr);
	k_printstr(numStr, 12, 18);
	num++;
	if(num > 1000){
	    num = 0;
	    k_printstr("    ", 12, 18); // To get rid of the extra numbers
        }
    }
}

void p2(){
    print_border(15, 10, 18, 35);
    k_printstr("Process 1 running...", 16, 11);
    uint32_t num = 0;
    k_printstr("value: ", 17, 11);
    while (1){
    	char numStr[5] = "";
        convert_num(num, numStr);
	k_printstr(numStr, 17, 18);
	num++;
	if(num > 1000){
	    num = 0;
	    k_printstr("    ", 17, 18); // To get rid of the extra numbers
        }
    }
}


















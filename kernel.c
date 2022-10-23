/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

#include <stdint.h>

// global variables
uint32_t pid;

// Dr. Roger's code
void run_test();
void buddy_init();
void *kmalloc(uint32_t);
int convert_num(unsigned int, char*);

void err();
struct idt_entry {
uint16_t lowoffset;
uint16_t code_selector;
uint8_t always0; 
uint8_t access;
uint16_t highoffset;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;
void init_idt_entry(idt_entry_t *entry, uint32_t addr_of_handler, uint16_t code_selector, uint8_t access);
void init_idt();
idt_entry_t idt[256];

struct idtr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// queue structs
// Node; process control block
typedef struct pcb_s{
    // Data
    uint32_t pid;
    uint32_t *esp;
    struct pcb_s *next;
    struct pcb_s *prev;
} pcb_t;
// Queue for above node
typedef struct {
    pcb_t *front;
    pcb_t *end;
} pcbq_t;
// Declaration
pcbq_t readyQueue;

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
void go();
void dispatch();
void dispatch_leave();
void lidtr(struct idtr_struct*);

int main(){
    buddy_init();
    k_clearscr();
    print_border(0,0,24,79);
    k_printstr("Running Processes", 1, 1);
    //p1();
    //p2();
    init_idt();
    if (create_process((uint32_t)&p1) == -1 || 
        create_process((uint32_t)&p2) == -1){
        k_printstr("An Error has occured with process 2",3,1);
        while(1){} // Keeps the code from proceeding in the event of an error
    }
    go();
    while(1){} // This keeps the screen from flickering
    return 0;
}

void enqueue(pcbq_t *q, pcb_t *pcb){
    if (q->end == 0){
        q->front = pcb;
        q->end = pcb;
    }
    else{
        pcb -> next = q -> end;
        q->end->prev = pcb;
        q->end = pcb;
    }
}

pcb_t *dequeue(pcbq_t *q){
    pcb_t *temp;
    temp = q->front;
    if (q->front == q->end){
        q->front = 0;
        q->end = 0;
        return temp;
    }
    q->front = q->front->prev;
    q->front->next = 0;
    return temp;
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

    uint32_t *st = stackptr+1024;

    st--; // Moving the mem location down 1
    uint32_t eflags = 0;
    *st = eflags; // Setting current mem location in stack to eflags(0)

    st--;
    uint32_t cs = 0;
    *st = cs; // code segment register

    st--;
    *st = code_address;

    st--;
    *st = dispatch_leave;

    // Setting registers to be 0
    // pop
    for (int i = 0; i < 8; i++){
        st--;
        *st = 0;
    }

    pid++;

    pcb_t *pcb = kmalloc(sizeof(pcb_t));
    pcb->esp = st;
    pcb->pid = pid;
    enqueue(&readyQueue,pcb);    

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
        asm("int $32");
    }
}

void p2(){
    print_border(15, 10, 18, 35);
    k_printstr("Process 2 running...", 16, 11);
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
        asm("int $32");
    }
}

void error(){
    k_printstr("ERROR", 0, 2);
    while(1){}
}

void init_idt_entry(idt_entry_t *entry, uint32_t addr_of_handler, uint16_t code_selector, uint8_t access){
entry->access = access;
entry->code_selector = code_selector;
entry->lowoffset = addr_of_handler & 0xFFFF;
entry->highoffset = addr_of_handler >> 16;
entry->always0 = 0;
}
void init_idt(){
    for (int i = 0; i < 32; i++){
        init_idt_entry(&idt[i], (uint32_t)&error, 16, 0x8e);
    }
    init_idt_entry(&idt[32], (uint32_t)&dispatch,16,0x8e);
    for (int i = 33; i < 256; i++){
        init_idt_entry(&idt[i],0,0,0);
    }
    struct idtr_struct idtr;
    idtr.limit = sizeof(idt)-1;
    idtr.base = (uint32_t)idt;
    lidtr(&idtr);
}

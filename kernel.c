/* Avery Kerley
   This program simply runs creates clears the screen, sets a border, and
   does the designated tests */

#include <stdint.h>
#include <limits.h>

// global variables
uint32_t pid;

// Dr. Roger's code
void run_test();
void buddy_init();
void *kmalloc(uint32_t);
int convert_num(unsigned int, char*);

void error();
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
    uint32_t priority;
    struct pcb_s *next;
    struct pcb_s *prev;
} pcb_t;
// Queue for above node
typedef struct {
    pcb_t *front;
    pcb_t *end;
} pcbq_t;
// Declaration
pcbq_t highPriorityQueue;
pcbq_t midPriorityQueue;
pcbq_t lowPriorityQueue;


// queue prototypes
void enqueue(pcbq_t *q, pcb_t *pcb);
pcb_t *dequeue(pcbq_t *q);

// text prototypes
void k_printstr(char *string, int row, int col);
void k_clearscr();
void print_border(int start_row, int start_col, int end_row, int end_col);

// process prototypes
int create_process(uint32_t code_address, uint32_t priority);
void idle();
void p1();
void p2();
void p3();
void go();
void dispatch();
void dispatch_leave();
void lidtr(struct idtr_struct*);

// Timer prototypes
void setup_PIC();
void init_timer_dev(uint32_t time_interval);
void outportb(uint16_t port, uint8_t value);

int main(){
    buddy_init();
    k_clearscr();
    print_border(0,0,24,79);
    k_printstr("Running Processes...", 1, 1);
    //p1();
    //p2();
    uint32_t time_interval;
    time_interval = 50; // in ms
    init_idt();
    init_timer_dev(time_interval);
    setup_PIC();
    if (create_process((uint32_t)&idle, 1) == -1 ||
        create_process((uint32_t)&p1, 2) == -1 ||
        create_process((uint32_t)&p2, 2) == -1 ||
        create_process((uint32_t)&p3, 3) == -1){
        k_printstr("An Error has occured with process generation",3,1);
        while(1){} // Keeps the code from proceeding in the event of an error
    }
    go();
    while(1){} // This keeps the screen from flickering
    return 0;
}

void enqueue_priority(pcbq_t *q, pcb_t *pcb){
    if (pcb->priority == 3)
        q = &highPriorityQueue;
    else if (pcb->priority == 2)
        q = &midPriorityQueue;

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

pcb_t *dequeue_priority(pcbq_t *q){
    if (highPriorityQueue.front)
        q = &highPriorityQueue;
    else if (midPriorityQueue.front)
        q = &midPriorityQueue;
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

int create_process(uint32_t code_address, uint32_t priority){
    uint32_t *stackptr = kmalloc(1024*sizeof(uint32_t));

    if (stackptr == 0)
        return -1;

    uint32_t *st = stackptr+1024;

    st--;
    *st = (uint32_t)&go;

    st--; // Moving the mem location down 1
    uint32_t eflags = 0x200;
    //uint32_t eflags = 0;
    *st = eflags; // Setting current mem location in stack to eflags(0)

    st--;
    uint32_t cs = 16; // I'm assuming because the others were 16, this is too
    *st = cs; // code segment register

    st--;
    *st = code_address;

    st--;
    *st = (uint32_t)&dispatch_leave;

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
    pcb->priority = priority;

    enqueue_priority(&lowPriorityQueue, pcb);

    // enqueue(&readyQueue,pcb);

    return 0;
}

void idle(){
    k_printstr("Process Idle running...", 24,1);
    while(1){
        k_printstr("/",24,24);
        k_printstr("\\",24,24);
    }
}

void p1(){
    print_border(10, 10, 13, 35);
    k_printstr("Process 1 running...", 11, 11);
    uint32_t num = 0;
    k_printstr("value: ", 12, 11);
    int i = 10000;
    while (i){
    	char numStr[5] = "";
        convert_num(num, numStr);
	k_printstr(numStr, 12, 18);
	num++;
	if(num > 1000){
	    num = 0;
	    k_printstr("    ", 12, 18); // To get rid of the extra numbers
        i--;
        }
    }
}

void p2(){
    print_border(15, 10, 18, 35);
    k_printstr("Process 2 running...", 16, 11);
    uint32_t num = 0;
    k_printstr("value: ", 17, 11);
    int i = 10000;
    while (i){
    	char numStr[5] = "";
        convert_num(num, numStr);
	k_printstr(numStr, 17, 18);
	num++;
	if(num > 1000){
	    num = 0;
	    k_printstr("    ", 17, 18); // To get rid of the extra numbers
        i--;
        }
    }
}

void p3(){
    print_border(20, 10, 23, 35);
    k_printstr("Process 3 running...", 21, 11);
    uint32_t num = 0;
    k_printstr("value: ", 22, 11);
    int i = 10000;
    while (i){
    	char numStr[5] = "";
        convert_num(num, numStr);
	k_printstr(numStr, 22, 18);
	num++;
	if(num > 1000){
	    num = 0;
	    k_printstr("    ", 22, 18); // To get rid of the extra numbers
        i--;
        }
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

void setup_PIC() {
    // set up cascading mode:
    outportb(0x20, 0x11); // start 8259 master initialization
    outportb(0xA0, 0x11); // start 8259 slave initialization
    outportb(0x21, 0x20); // set master base interrupt vector (idt 32-38)
    outportb(0xA1, 0x28); // set slave base interrupt vector (idt 39-45)
    // Tell the master that he has a slave:
    outportb(0x21, 0x04); // set cascade ...
    outportb(0xA1, 0x02); // on IRQ2
    // Enabled 8086 mode:
    outportb(0x21, 0x01); // finish 8259 initialization
    outportb(0xA1, 0x01);
    // Reset the IRQ masks
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
    // Now, enable the clock IRQ only
    outportb(0x21, 0xfe); // Turn on the clock IRQ
    outportb(0xA1, 0xff); // Turn off all others
}

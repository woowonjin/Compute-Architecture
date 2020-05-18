#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NUMMEMORY 65536
#define NUMREGS 8
#define MAXLINELENGTH 1000

typedef struct stateStruct{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

int convertNum(int);    
void machineToAssembler(int, int*, int*, int*, int*);
void printState(stateType*);
int main(int argc, char* argv[]){
    char line[MAXLINELENGTH];
    stateType state;
    FILE* filePtr;

    if(argc != 2){
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if(filePtr == NULL){
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    for(state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++){
        if(sscanf(line, "%d", state.mem+state.numMemory) != 1){
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);

    }
    state.pc = 0;
    for(int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0;
    }
    int op, reg0, reg1, off;
    int count = 0;
    while(1){
        printState(&state);
        machineToAssembler(state.mem[state.pc], &op, &reg0, &reg1, &off);
        if(op == 0){ //add op
            state.reg[off] = state.reg[reg0] + state.reg[reg1];
        }
        else if(op == 1){ // nor op
            state.reg[off] = ~(state.reg[reg0] | state.reg[reg1]);
        }
        else if(op == 2){ // lw op
         //   printf("off :%d\n", off);
           // printf("contents of reg0 : %d\n", state.reg[reg0]);
            //printf("address of reg1 : %d\n", reg1);
            state.reg[reg1] = state.mem[state.reg[reg0] + off];
        }
        else if(op == 3){ // sw op
            state.mem[state.reg[reg0] + off] = state.reg[reg1];
        }
        else if(op == 4){ // beq op
            if(state.reg[reg0] == state.reg[reg1]){
                state.pc += off;
            }
        }
        else if(op == 5){ // jalr op
            state.reg[reg1] = state.pc + 1;
            state.pc = state.reg[reg0] - 1;
        }
        else if(op == 6){ // halt op
            printf("\nmachine halted\n");
            printf("total of %d instructions executed\n", ++count);
            printf("final state of machine:");
            state.pc++;
            printState(&state);
            break;
        }
        else if(op == 7){ // noop op
        }
        else{
            printf("Error : opcode does not exists!!\n");
            exit(1);
        }
        state.pc++;
        count++;
    }


    return 0;
}

void printState(stateType* statePtr){
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for(i = 0; i < statePtr->numMemory; i++){
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for(i = 0; i < NUMREGS; i++){
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state");
}

void machineToAssembler(int target, int* op, int* reg0, int* reg1, int* off){
    *op = (target >> 22) & 7;
    *reg0 = (target >> 19) & 7;
    *reg1 = (target >> 16) & 7;
    *off = convertNum(target & 65535);
}

int convertNum(int num){
    if(num & (1<<15)){
        num -= (1<<16);
    }
    return num;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINELENGTH 1000
int getAddress(char*);
int readAndParse(FILE*, char*, char*, char*, char*, char*);
int isNumber(char*);

static char* inFile;

int main(int argc, char* argv[]){
    char* inFileString;
    char* outFileString;
    FILE* inFilePtr;
    FILE* outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    if(argc != 3){
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",argv[0]);
        exit(1);
    }
    inFileString = argv[1];
    inFile = inFileString;
    outFileString = argv[2];
    inFilePtr = fopen(inFileString, "r");
    if(inFilePtr == NULL){
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if(outFilePtr == NULL){
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    FILE* tempInFilePtr = fopen(inFileString, "r");
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if(strcmp(label, "") != 0){
            int count = 0;
            char tempLabel[MAXLINELENGTH];
            while(readAndParse(tempInFilePtr, tempLabel, opcode, arg0, arg1, arg2)){
                if(strcmp(tempLabel, label) == 0){
                    count++;
                }
            }
            if(count > 1){
                printf("Error : Duplicate label\n");
                exit(1);
            }
            rewind(tempInFilePtr);
        }
    }
    fclose(tempInFilePtr);
    rewind(inFilePtr);
    int PC = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        int arg0Num, arg1Num, arg2Num, op, off, machineCode;
        if(strcmp(opcode, "add") == 0){
            op = 0;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            arg2Num = atoi(arg2);
            off = arg2Num;
        }
        else if(strcmp(opcode, "nor") == 0){
            op = 1;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            arg2Num = atoi(arg2);
            off = arg2Num;
        }
        else if(strcmp(opcode, "lw") == 0){
            op = 2;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            if(isNumber(arg2)){
                off = atoi(arg2);
            }
            else{
                off = getAddress(arg2);
            }
        }
        else if(strcmp(opcode, "sw") == 0){
            op = 3;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            if(isNumber(arg2)){
                off = arg0Num + atoi(arg2);
            }
            else{
                off = arg0Num + getAddress(arg2);
            }
        }
        else if(strcmp(opcode, "beq") == 0){
            op = 4;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            if(isNumber(arg2)){
                off = arg0Num + atoi(arg2);
            }
            else{
                off = getAddress(arg2) - PC - 1;
            }
        }
        else if(strcmp(opcode, "jalr") == 0){
            op = 5;
            arg0Num = atoi(arg0);
            arg1Num = atoi(arg1);
            off = 0;
        }
        else if(strcmp(opcode, "halt") == 0){
            op = 6;
            arg0Num = 0;
            arg1Num = 0;
            off = 0;
        }
        else if(strcmp(opcode, "noop") == 0){
            op = 7;
            arg0Num = 0;
            arg1Num = 0;
            off = 0;
        }
        else if(strcmp(opcode, ".fill") == 0){
            if(isNumber(arg0)){
                machineCode = atoi(arg0);
                fprintf(outFilePtr, "%d\n", machineCode);
                PC++;
                continue;
            }
            else{
                machineCode = getAddress(arg0);
                fprintf(outFilePtr, "%d\n", machineCode);
                PC++;
                continue;
            }
        }
        else{
            printf("Error : this opcode does not exists!\n");
            exit(1);
        }
        PC++;
        if(off < -32768 || off > 32767){
            printf("Error : Offset is overflowed\n");
            exit(1);
        }
        if(off < 0){
            off += 65536;
        }
        machineCode = (op<<22) + (arg0Num<<19) + (arg1Num << 16) + off;
        fprintf(outFilePtr, "%d\n", machineCode);
    }
    fclose(inFilePtr);
    fclose(outFilePtr);
    exit(0);
}

int readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0, char* arg1, char* arg2){
    char line[MAXLINELENGTH];
    char* ptr = line;
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
    if(fgets(line, MAXLINELENGTH, inFilePtr) == NULL){
        return 0;
    }

    if(strchr(line, '\n') == NULL){
        printf("error: line too ling\n");
        exit(1);
    }
    ptr = line;
    if(sscanf(ptr, "%[^\t\n\r ]", label)){
        ptr += strlen(label);
    }

    sscanf(ptr, "%*[\t\n\r  ]%[^\t\n\r  ]%*[\t\n\r  ]%[^\t\n\r  ]%*[\t\n\r  ]%[^\t\n\r  ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
    return(1);
}

int getAddress(char* targetLabel){
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    FILE* tempInFilePtr = fopen(inFile, "r");
    int address = 0;
    while(readAndParse(tempInFilePtr, label, opcode, arg0, arg1, arg2)){
        if(strcmp(targetLabel, label) == 0){
            fclose(tempInFilePtr);
            return address;
        }
        address++;
    }
    fclose(tempInFilePtr);
    printf("Error : Can't find label\n");
    exit(1);
}

int isNumber(char* string){
    int i;
    return((sscanf(string, "%d", &i)) == 1);
}

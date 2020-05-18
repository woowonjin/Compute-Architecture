        lw  0   1   five
        lw  0   2   neg1
start   jalr    7   7
        add 1   2   1
        beq 0   1   2
        beq 0   0   start
        jalr    7   7
        halt
five    .fill   5
neg1    .fill   -1

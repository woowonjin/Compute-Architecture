        lw  0   0   zero
        lw  0   1   1
        beq 0   1   No
        add 1   1   2
No      halt
zero    .fill   0

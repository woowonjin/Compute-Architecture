        lw  0   0   zero
        lw  0   1   1
        nor 0   1   2
        beq 2   0   done
        add 1   1   1
done    halt
zero    .fill   0

        lw  0   0   zero
        lw  0   1   zero    result register
        lw  0   2   index   compare where to add
        lw  0   3   zero    check whether 1 or 0 in index bit of mplier & index
        lw  0   4   mcand
        lw  0   5   mplier
        lw  0   6   max     store 1000 0000 0000 0000
        lw  0   7   zero    Nothing
loop    nor 5   5   5       store 1's complement to mplier
        nor 2   2   2       stroe 1's complement to index
        nor 5   2   3       calculate mplier & index
        nor 5   5   5       restore mplier
        nor 2   2   2       restore index
        beq 0   3   bitZero check the index bit of mplier is 0 or 1
        add 4   1   1
bitZero add 4   4   4       mcand left shift one bit
        add 2   2   2       index left shift one bit
        beq 2   6   done
        beq 0   0   loop
mcand   .fill   32766
mplier  .fill   10838
zero    .fill   0
max     .fill   32768
done    halt

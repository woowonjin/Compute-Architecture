	lw 0 1 five	reg 1 has a 5
	lw 0 2 five	reg 2 has a 5
	lw 0 3 one	reg 3 has a 0
	nor 1 2 4	reg 4 has a 5 nor 5
	add 4 3 4	reg 4 is a reverse of reg 1 value
done	halt
five	.fill 5
one	.fill 1

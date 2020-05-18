	lw 0 1 14	Load 15 to reg 1 (numeric)
	lw 0 2 neg2	load -2 to reg 2 (symbolic)
	lw 0 3 zero	load 0 to reg 3 (symbolic)
	lw 0 4 zero	load 0 to reg 4 (symbolic)
	lw 0 5 one	load 1 to reg 5 (symbolic)
start	add 1 2 1	add %1 + %2 -> %1
	add 5 3 3	add %5 + %3 -> %3
	beq 5 1 1	if %1 == 1 break
	beq 0 0 start go to loop
	add 4 1 4	add %4 + %1 = %4
	noop 
done halt 
zero .fill 0 
one .fill 1 
Fteen .fill 15 
neg2 .fill -2

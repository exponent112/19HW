.data
digit:   .word 10, 12, 23, 28, 7, 39, 10, 11, 23, 12, 3, 4, 5, 1, 34, 17, 0, 5, 24
length:   .word 19 # the length of the digit list
str: .asciiz "sum is "
mstr: .asciiz "min is "
xstr: .asciiz "max is "



.text
main:

addi $t5, 113 #t5 = q
addi $t6, 115 #t6 = s
addi $t7, 120 #t7 = x
addi $t8, 110 #t8 = n
start:
move $t2, $zero # t2 = 0
li $v0, 12
syscall
move $t4, $v0
move $s3, $zero #i = 0
la $s6, digit
la $t3, length
lw $t3, 0($t3)
beq $t4, $t5, exit
beq $t4, $t6, Loop
beq $t4, $t7, fmax
beq $t4, $t8, ffmin





Loop:
beq  $s3, $t3, padd
sll  $t1, $s3, 2
add  $t1, $t1, $s6   #t1 = &save[i]
lw   $t6, 0($t1)
add  $t2, $t2, $t6
addi $s3, $s3, 1
j    Loop

padd:
li $v0, 4 #print sum is $t0
la $a0, str
syscall
li $v0, 1
move $a0 ,$t2
syscall #print sum is $t0
j start


fmax:
beq  $s3, $t3, pmax
sll  $t1, $s3, 2
add  $t1, $t1, $s6   #t1 = &save[i]
lw   $t6, 0($t1)
slt   $s1, $t2, $t6
bne   $s1, $zero, equ
jmax:
addi $s3, $s3, 1
j    fmax

equ:
move $t2, $t6
j    jmax

pmax:
li $v0, 4 #print max is $t0
la $a0, xstr
syscall
li $v0, 1
move $a0 ,$t2
syscall #print max is $t2
j start

ffmin:
addi $t2, $zero, 100
fmin:
beq  $s3, $t3, pmin
sll  $t1, $s3, 2
add  $t1, $t1, $s6   #t1 = &save[i]
lw   $t6, 0($t1)
slt   $s1, $t6, $t2
bne   $s1, $zero, equ2
jmin:
addi $s3, $s3, 1
j    fmin

equ2:
move $t2, $t6
j    jmin

pmin:
li $v0, 4 #print max is $t0
la $a0, xstr
syscall
li $v0, 1
move $a0 ,$t2
syscall #print max is $t2
j start

# HERE, implement mips code
# to get the sum, max, and min of the ‘digit’ list above
# and to print the results (sum, max, and min)

exit:
li $v0, 10 #quit
syscall #quit
.end

@R1
D=M
@i
M=D
@R2
M=0

(LOOP)
@i
D=M
@END
D;JEQ

@R0s
D=M
@R2
M=D+M

@i
M=M-1

@LOOP
0;JMP

(END)
@END
0;JMP
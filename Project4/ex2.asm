@i
M=0
(LOOP)

	@i
	D=M
	@R1
	D=D-M
	@END
	D;JEQ
	@R0
	D=M
	@i
	A=D+M
	M=-1
	@i
	M=M+1
	@LOOP
	0;JMP

(END)
	@END
	0;JMP

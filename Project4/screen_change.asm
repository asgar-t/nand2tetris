//Storing KBD-1 in R0
@KBD
D=A
@0
D=D-1
M=D

(MAIN)

	//IF Key is pressed, go to black, otherwise go to white, if some error (Negative value) return to main
	@KBD
	D=M

	@BLACK
	D;JGT

	@WHITE
	D;JEQ


	@MAIN
	0;JMP


(WHITE)

	@SCREEN //Sets i to starting screen address
	D=A
	@i
	M=D

	(LOOP_WHITE)
		//if i reaches last screen registoer (KBD-1), go to main
		@i
		D=M
		@0
		D=D-M
		@MAIN
		D;JEQ

		//sets 16 pixels to white
		@i
		A=M
		M=0
		//next register
		@i
		M=M+1

		@LOOP_WHITE //continue loop
		0;JMP


//Same as white but for black
(BLACK)
	@SCREEN
	D=A
	@i
	M=D

		(LOOP_BLACK)

		@i
		D=M
		@0
		D=D-M
		@MAIN
		D;JEQ

		@i
		A=M
		M=-1

		@i
		M=M+1

		@LOOP_BLACK
		0;JMP
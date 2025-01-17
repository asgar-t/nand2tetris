(START)
	@R0 
	D=M
	@R2
	M=D  //stpres r0 in r2
	@R1
	D=D-M

	@R	
	D;JLT //if r1 > r2
	
	@END //else end
	0;JMP
	
	
	



(R1_GT)
	
	@R1 //stores r1 in r2
	D=M
	@R2
	M=D

(END) //end
	@END
	0;JMP



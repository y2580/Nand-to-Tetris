// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.


(Listen)
@KBD
D=M
@BLACK
D;JNE
@WHITE
0;JMP

(BLACK)
@i
M=0
    (BLACKLOOP)
    @i
    D=M
    @8192
    D=D-A
    @Listen
    D;JEQ
    @SCREEN
    D=A
    @i
    A=D+M
    M=-1
    @i
    M=M+1
    @BLACKLOOP
    0;JMP

(WHITE)
@i
M=0
    (WHITELOOP)
    @i
    D=M
    @8192
    D=D-A
    @Listen
    D;JEQ
    @SCREEN
    D=A
    @i
    A=D+M
    M=0
    @i
    M=M+1
@WHITELOOP
0;JMP
## Lets build a compiler
Adapted from "Let's Build a Compiler, by Jack Crenshaw" http://compilers.iecc.com/crenshaw/ 
This implementation is done in C from scratch.
## Progress
- 5/30/2015 Part 1 has been implemented (http://compilers.iecc.com/crenshaw/tutor1.txt

##Output
The output is some kind of assembly. A subset of IBM assembly most likely.

## Registers
There is 15 General purpose registers R0 to R14.

##Instruction Set
- `LA RX <64 BIT INTEGER>`
Load the contents of the right operand into the register RX.

- `LR RX RX`
Load the contents of the right operand (register) into the left operand (register).

- `AR RX RX`
Add the contents of the left operand and the right operand and store the sum in 
the right operand.

- `SR RX RX`
Subtract the contents of the left operand from the right operand and store the difference in 
the right operand.

##Examples (5/30/2015)
- `./main "1000+2+15"`
- `./main "2000-15+5"`



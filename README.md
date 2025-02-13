# Regular Expression to DFA Conversion Using Syntax Tree Approach

## Overview
This project implements the conversion of a regular expression into a Deterministic Finite Automaton (DFA) and verifies whether a given input string is accepted by the DFA.

## Approach
The implementation follows a structured pipeline:

### 1. Regex Modification & Postfix Conversion
- The input regular expression is modified to ensure explicit concatenation (`.`) where necessary.
- The modified expression is then converted into postfix notation using a stack-based approach.

### 2. Syntax Tree Construction
- A syntax tree is constructed using the postfix expression.
- The tree is traversed to compute:
  - `nullable` properties.
  - `firstpos` and `lastpos` sets.
  - `followpos` table, which maps positions to subsequent positions.

### 3. DFA Construction
- DFA states are created using `firstpos` of the root as the initial state.
- Transitions are derived based on `followpos` mapping.
- Final states are determined by checking if a state contains the position of the terminal symbol (`#`).

### 4. DFA Simulation
- The constructed DFA is used to process an input string.
- The simulation tracks state transitions and determines acceptance based on reaching a final state.

## Example Output
```
Enter Regular Expression: (a|b)*abb
DFA Details :
States: { 0 1 2 3 }
Alphabets: { a b }
Initial State: 0
Final States: { 3 }
Transition Table:
  Current   Input   Next     
       0        a       1     
       0        b       0     
       1        a       1     
       1        b       2     
       2        a       1     
       2        b       3     
       3        a       1     
       3        b       0     

Enter Input String: abbbabb
  Current State   Input   Next  
          0         a      1      
          1         b      2      
          2         b      3      
          3         b      0      
          0         a      1      
          1         b      2      
          2         b      3      
Result: ACCEPTED
```

## Implementation Details
- **Regex Parsing:** Converts infix regex to postfix for easier processing.
- **Syntax Tree:** Used to extract `nullable`, `firstpos`, `lastpos`, and `followpos`.
- **DFA Construction:** Utilizes `followpos` to generate DFA states and transitions.
- **Simulation:** Processes the input string through the DFA to determine acceptance.

This implementation effectively demonstrates the transformation of regular expressions into deterministic finite automata while handling concatenation, alternation, and Kleene closure.


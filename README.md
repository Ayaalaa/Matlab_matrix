ASU-Math-Lab-Project-detailed-description.
==========================================
Intro
-----
ASU-Math-Lab-Project aims to Implement mathematical library software
Similar to Matlab, Octave and similar tools.

Environment
-----------
This is a complete C/C++ Implementation that developed under Linux environment.

Phases
------
It consists of three phases:

- Phase 1: Core Operations
- Phase 2: Advanced Operations and Tuning 

Phase 1 Requirements
------------------------
- Implement C++ class for matrix
- Support dynamic creation and destruction of matrices of any size
- Support addition, subtraction, multiplication, transpose and division
- Process input user commands and show results directly 
- Process input file, show each step result then exit
Supported Operations
--------------------
- add ( + )
- sub ( - )
- mult ( * )
- div ( / )
- elementWiseDiv ( ./ )
- cascaded equal
Operations Example
------------------
::

    > A = [1.4 2.2 3.2; 4.4 5.4 6.4; 3.3 4.2 2.2];
    > B = [1.5 4.1 5.4; 3.1 4.2 1.2; 3.2 4.3 2.2];
    > C = A + B
    > D = A - B
    > E = A * B
    > F = A / B
    > G = A’

Phase 2: Advanced Operations and Tuning 
---------------------------------------
- Support mathematical functions.
    Trigonometric, Logarithmic, Roots, Power
- Support mathematical expressions.
    Either with (dot) or not.
- Support flexible matrix parser.
    Accept matrix in matrix, expressions and variables.
- Support error handling.
    Do not crash for invalid input.
Supported Operations
--------------------
- supported operations in phase 1
- Sine 
- Power
- Sqrt
- Log
- matrix parser
- rand( , ), eye( , ), zeros( , )and ones( , )
Operations Example
------------------
::

    A = [1.4 2.2 3.2; 4.4 5.4 6.4; 3.3 4.2 2.2];
    B = [1.5 4.1 5.4; 3.1 4.2 1.2; 3.2 4.3 2.2];
    A = 5.5 + 12 * sin(0.4) + 2.2^4;
    B = [1.2 2.3 A;[1.3 2.4;4.6 1.3],[3.2;7.8]];
    C = [[B [3.4; 2.1; 3.5+9.1]] 1.2^3 3+1.2 15/(2.1+10*sin(0.12))  1.2]
    D = rand(4,4)
    E = eye(4, 4)
    F = zeros(2, 3)
    G = ones(3, 6)
    L = (1.2 + 3.4 - 5.6)/(2.1*3.2 + 4.6) - 12.1*3.1 + (1.2 + 5.2)^(4/(3.2+5.6))
    X = ((C*D .+ 4)./2.1 + sqrt(D))./C.^2
    Y = (C^3 * sin(1./D))^(0.1)




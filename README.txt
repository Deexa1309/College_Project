
Relocating SIC assembler using bitmask

To execute the assembler, follow any one of the three instructions:
1. To execute though command line by giving text file as input,
        -> g++ code.cpp -o main
        -> .\main "input1.txt"

2. To execute though command line normally,
    a) g++ code.cpp
    b) .\a

3.  Run code.cpp in an IDE

Two text files will be generated
1.  record.txt - contains object program.
2.  finalobjectcode.txt - contains object program with relocation by bitmask.

input1.txt contains COPY.   (Program to read record from input device and write in output device)
input2.txt contains ARRSUM. (Program to sum two arrays and store in 3rd array)
input3.txt contains COUNT.  (Program to count number of zeroes in an array)

# Boyer-Moore-string-search-algorithm
Boyer-Moore string search algorithm
This project implements the Boyer-Moore string search algorithm, a highly efficient pattern matching technique used to find occurrences of a substring (pattern) within a larger string (text).

The program reads multiple test cases from an input file and applies the Boyer-Moore algorithm using both Bad Character Heuristic and Good Suffix Heuristic to minimize character comparisons.

Features:
Efficient preprocessing using:

Bad Character Shift Table (BCST)

Good Suffix Shift Table (GSST)

Accurate pattern matching with optimized character comparisons

Records:

All matching positions of the pattern

Number of comparisons made

Time taken for each search

Outputs results and performance d

File Structure:
boyermoore_PES2UG23CS384.c: Main implementation

input.txt: Input file containing test cases (text and pattern)

boyermoore_output.txt: Output with detailed matching steps

boyermoore_values.txt: Summary log with pattern length, text length, comparisons, and time taken

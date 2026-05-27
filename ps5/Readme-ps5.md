# PS5: DNA Alignment

## Contact
Name: Salma Boukdad
Section: 203

Time to Complete: 15 hrs

## Description
This program computes the optimal sequence alignment between two DNA strings using
dynamic programming. It calculates the minimum edit distance between the strings by
filling an opt matrix with alignment costs, then traces back through the matrix to
recover the actual alignment. It reads two strings from stdin and outputs the
edit distance, the alignment, and the execution time.

### Features
The alignment is computed using the Needleman-Wunsch dynamic programming algorithm.
The core data structure is a 2d matrix stored as an
std::vector<std::vector<int>>, allocated dynamically after reading the input strings
so the size can be determined at runtime. The matrix is filled bottom-right
to top-left using the recurrence: 
opt[i][j] = min(opt[i+1][j+1] + penalty(x[i], y[j]), opt[i+1][j] + 2, opt[i][j+1] + 2)
with base cases opt[M][j] = 2*(N-j) and opt[i][N] = 2*(M-i).
The alignment is recovered by tracing the matrix from opt[0][0] to opt[M][N],
checking which of the three cases produced each cell's value.

### Testing
TestConstructor: verifies that constructing an EDistance object with an empty
string throws std::invalid_argument, and that valid strings do not throw.

TestPenalty: verifies that penalty('A', 'T') returns 1 (mismatch) and 
penalty('C', 'C') returns 0 (match).

TestMin3: verifies that min3 correctly returns the minimum of three integers
across several cases.

TestOptDistance: verifies that the edit distance between "AACAGTTACC" and 
"TAAGGTCA" is 7, matching the expected result from the spec.

TestAlignment: verifies that the total cost of all rows in the alignment
string sums to 7, rather than checking the exact string, since multiple valid 
alignments exist.

### Issues
The trickiest part was handling Windows-style \r\n line endings in the input files.
Without stripping the \r character after getline, the edit distances were slightly
inflated for some ecoli files. Adding a check for s.back() == '\r' after each
getline fixed the issue. Dynamic memory allocation for the matrix was also a
learning point, where a static 2D array could not be used when the dimensions
are only known at runtime.

## Analysis
### Example
Ex Test Case: A corner case with two identical strings

Input:
    ACGT
    ACGT

Output: 
    Edit distance = 0
    A A 0
    C C 0
    G G 0
    T T 0
    Execution time is 4.8e-05 seconds

The program ran correctly on this input.

### Specs
My Computer
Memory: 10.5 GB
Processors: Intel Core i7-9750H @ 2.60GHz

### Runs
| data file     | distance | memory (MiB) | time (seconds) |
|---------------|----------|--------------|----------------|
|ecoli2500.txt  |    118   |   24.07      |    0.672882    |
|ecoli5000.txt  |    160   |   95.78      |    2.65304     |
|ecoli7000.txt  |    194   |   187.5      |    5.19804     |
|ecoli10000.txt |    223   |   382.2      |    11.0703     |
|ecoli20000.txt |    3135  |   1491       |    43.6632     |
|ecoli28284.txt |    8394  |   3080       |    82.104      | 
|ecoli50000.txt |   19487  |   9743       |    301.575     |
|ecoli100000.txt|     NA   | exceeds mem  |      NA        |

### Time
Assume the two strings are the same length (M = N).  By applying the doubling method to the data points that you obtained, estimate the running time of your program in seconds as a polynomial function of N, in the form a * N^b for some constants a and b.  Determine the largest input your program can handle if it is limited to one day of computation.

Provide a brief justification/explanation of how you applied the doubling method, and if your data seems not to work, describe what went wrong and use the sample data instead.
 - a = 0.67 / (2500^2) = 0.67 / 6250000 = 0.000000107 --> a = 1.07 * 10^-7
 - b = 2
 - largest N = 86400 = 1.07e-7 * N^2 : N^2 = 86400 / 1.07e-7 = 808 billion : N = ~899,000

The doubling method was applied by observing how execution time scales when N doubles. From N = 2500 to N = 5000,
time increased from 0.67s to 2.65s - a ratio of ~3.96.
From N = 5000 to N = 10000, time increased from 2.65s to 11.07s - a ratio of ~4.18.
Since doubling N multiplies time by ~4, and 2^b = 4, b = 2, confirming O(N^2) 
growth as expected for a DP algorithm filling an N x N matrix.

### Memory
Assume the two strings are the same length (M = N).  Look at your code and determine how much memory it requires as a polynomial function of N, in the form a * N^b for some constants a and b.  Determine the largest input size your computer can handle if limited to 8GiB of RAM. (8GiB = 8192 MiB)
 - memory = 4 * N^2 / (1024^2)
 - a = 4/1048576 = 3.81 * 10^-6
 - b = 2
 - largest N: 8192 = 3.81e-6 * N^2 : N^2 = 8192 / 3.81e-6 = 2.15 billion : N = ~46,340

### Valgrind
| data file     | Calculated (MiB) | Measured (MiB) | Difference (%) |
|---------------|------------------|----------------|----------------|
|ecoli2500.txt  |      23.84       |      24.07     |       1%       |
|ecoli5000.txt  |      95.37       |      95.78     |      0.4%      |
|ecoli7000.txt  |      186.85      |      187.5     |      0.35%     |
|ecoli10000.txt |      381.47      |      382.2     |      0.2%      |
|ecoli20000.txt |      1525.88     |      1527      |      0.1%      |
|ecoli28284.txt |      3051.56     |      3080.5    |      0.95%     |
|ecoli50000.txt |      9536.74     |      9743      |       2%       |
|ecoli100000.txt|      38146.97    |   exceeds mem  |      N/A       |

No errors were detected by valgrind. The measured memory closely matches the
calculated values, confirming that the opt matrix dominated memory usage
and there are no leaks.

## Pair Programming
I worked alone.

## Extra Credit
manual min implementation: 7.07854 seconds
std::min implementation: 10.5848 seconds

manual min optimization levels
------------------------------
g++ -O0 ... -> 7.0582 sec.
g++ -O1 ... -> 0.446286 sec.
g++ -O2 ... -> 0.301925 sec.
g++ -O3 ... -> 0.358079 sec.

std::min optimization levels
----------------------------
g++ -O0 ... -> 10.5956 sec.
g++ -O1 ... -> 0.903524 sec.
g++ -O2 ... -> 0.363039 sec.
g++ -O3 ... -> 0.354302 sec.

- The manual min3 was faster than std::min at -O0, likely because std::min
with an initializer list was overhead from constructing a temporary list

- At -O1 and above both implementations got dramatically faster, with the gap
narrowing significantly

- At -O2 and -O3 the times converge, suggesting the compiler optimizes both
implementations similary at higher levels

- -O2 appears to be the sweet spot, -O3 didn't improve and was slightly slower
in some cases

## Acknowledgements
2d Vector in C++: https://www.geeksforgeeks.org/cpp/2d-vector-in-cpp-with-user-defined-size/


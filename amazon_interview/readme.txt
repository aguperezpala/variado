Problem Specification
----------------------

Create a command line application that takes 1 argument pointing to a file (file path).
The tool should read the file containing multiple lines where each line has the form:
                    <function name>: N1,N2,N3,N4...Nm\n
where <function name> could be {min, sum, avg, p90} and then a list of integers 
(N1,...,Nm) separated by a ‘,’. Each line ends with a ‘\n’ character.
For each line we should apply the respective function to the list of the integers 
and output the result in the standard out.


Possible problems
------------------

- Invalid file path.
- Syntax problems (for functions or integers).
- Big integers data set (a lot of integers as input without any possibility to 
  read all of them into the memory).
- Integers with size different than sizeof(int).
- Resulting values can cause overflows (sum of integers for example)
- UpperCase / LowerCase function names.
- What we should print as output data in erroneous cases... I put 3 different 
  kind of errors (EmptyLine: EL, Unknown Function Name: UFN, 
  Invalid Line Format: ILF, for when we read an empty line, a function that is 
  correctly formed but is not known and for a ill-formed line -parsing errors- 
  respectively).


Assumptions
-----------

- The integers are between the range of the int type in the architecture where 
  the tool is compiled (32 / 64 / X bits). This could be probably modified to 
  be a long long without problems (in the Defines.h file, changing the 
  typedef int IntegerType).
- Each line with information (that needs to be processed) finish with a ‘\n’ 
  character (inclusive the last one).
- Sum of values and any kind of operation being done with the input integers will 
  not cause “overflows”. If so, then I should add that checking and print an 
  specific error message or work with other data types (bigger than the current 
  integers).
- All the complications and things were thought taking into account that the input 
  data could be VERY BIG, so we need to create functions that can support 
  “streamed inputs” (this means that can be calculated incrementally, without 
  having all the data when they are evaluated).
- You have linux and a g++ 4.6.x (I’m sure that should compile for this compiler, 
  I’m using 4.7 but without any new feature of the new C++11). 



How to compile
---------------

We are using UnitTest++ lib as testing library [http://unittest-cpp.sourceforge.net/] 
and using a “not too nice” third party library named p2 [https://github.com/absmall/p2] 
already included in the project (used to calculate percentil90 for stream data based 
on the paper [http://www.cse.wustl.edu/~jain/papers/psqr.htm] (I hadn’t the right 
time to implement my own function based on some other articles that I read about 
how to calculate percentiles for big size data sets).

To compile the test you need to have UnitTest++ library installed on your system 
(but this is not necessary to run the tool itself). To build it you can run on a 
console (and in the folder where the project is):

~$ g++ -o test test.cpp FunctionBuilder.* StreamFunctions.* StreamEvaluator.* Parser.* extLibs/* -I. -std=c++0x -lUnitTest++ -g

To compile the tool itself you don’t need UnitTest++ so calling:

~$ g++ -o tool main.cpp FunctionBuilder.* StreamFunctions.* StreamEvaluator.* Parser.* extLibs/* -I. -std=c++0x -g

will compile the tool (named tool) in the same directory.

To execute the tool or the tests simply call 
~$ ./test
~$ ./tool 
in a console.


Comments about some modules / parts of the project
--------------------------------------------------

Parser
---------
Probably it seems a little bit complicated the code of the parser because we cannot ensure:
- Empty lines in the file (and we want to detect these cases to print as many lines 
  as we get in the input).
- We cannot ensure the files are well formed (empty spaces in any place, enormous 
  functions names, ill-formed numbers, etc).
- We can’t read a full line at once because we don’t know the size of each line 
  (probably too big to fit in memory).

Another way to parse all the data is using regular expressions (STL regex) but 
this will not be possible (in an easy way) because we cannot perform that operations 
over an istream... So, I decided to do it like in the old school parsing by hand...
We probably can create a wrapper that matches with the STL bidirectional iterator 
classes and move over the file as a simple std::string...
Using boost is not an option because is a big library and the implementation over 
istreams is probably reading all the data into a memory buffer and then apply the 
operations over there (with big data set we have the same problem)..


Percentile 90
--------------
Here we list all the possible solutions we can implement:

- First naive solution: Save all the data into a memory-allocated buffer, push all 
  the numbers we can from the stream into the buffer, if the buffer is full then 
  drop all the information into a temporary file and clear the buffer and continue 
  doing the same operations until we know that we have all the data. Then apply 
  “divide and conquer” over the saved data sorting the numbers and getting the p90 
  (VERY SLOW!).
- Second possible solution: Limit the size of the array in at least M, 
  where M = N * 10, where N is the size of preallocated memory we want to use 
  (ram memory), since we only need to allocate the 10% of the total numbers (we 
  can store only the biggest 10% of the numbers, and then get the associated p90 
  taking into account the total numbers allocated in the array and all the numbers 
  read from the stream).
- Sacrifice precision and give a approximated result (using some of the solutions 
  that work with sampling or anything else). This is the current approach, I’m not 
  very happy with this but I should maintain one of my assumptions (big input 
  data sets)
- Investigate a little more in depth about all the possible solutions and think 
  in a new algorithm to calculate this.
- Create a time machine to have more time and research / think in a probably 
  algorithm that can accomplish with precision + performance + memory constraints 
  (having more information about the problem itself will clarify this a lot).


Things to improve
------------------
- Parser using regular expressions or another lexer / parser.
- Check for possible overflows (when summing integers for example).
- Improve the design and how the files are organized. If we have more specifications 
  defined it will be much more easier.
- Since we are reading a file, this means that is not necessary a “live stream” 
  so we can read how many numbers will have a particular line (reading the ‘,’ for 
  example). This way we can make a much more clear assumptions about how to calculate 
  percentiles for example...
- The current UnitTest are grouped... they should be separated.. (I wrote less code 
  this way :p).
- It is possible to parallelize the computation of the input values since it is a 
  file -> we can split the file into blocks and run the StreamEvaluator with 
  different streams (streams pointing to the same file but different blocks).
- Customize data types and debug information output when compiling.
- Use Makefiles instead of ugly big lines to compile the project :(.



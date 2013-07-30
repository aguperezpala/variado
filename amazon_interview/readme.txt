Problem Specification
---------------------
Create a command line application that takes 1 argument pointing to a file (file path).
The tool should read the file containing multiple lines where each line has the form:
<function name>: N1,N2,N3,N4...Nm\n
where <function name> could be {min, sum, avg, p90} and then a list of integers (N1,...,Nm) separated by a ‘,’. Each line ends with a ‘\n’ character.
For each line we should apply the respective function to the list of the integers and output the result in the standard out.


Possible problems
-----------------
- Invalid file path.
- Syntax problems (for functions or integers).
- Big integers data set.
- Integers with size different than sizeof(int).
- Resulting values can cause overflows (sum of integers for example)
- UpperCase / LowerCase function names
- What we should print as output data in erroneous cases...


Parser
------
Probably it seems a little bit complicated the code of the parser because we cannot ensure:
- Empty lines in the file (and we want to detect these cases to print as many lines as we get in the input).
- We cannot ensure the files are well formed (empty spaces in any place, enormous functions names, ill-formed numbers, etc).
- We can’t read a full line at once because we don’t know the size of each line (probably too big to fit in memory).

Another way to parse all the data is using regular expressions (STL regex) but this will not be possible (in an easy way) because we cannot perform that operations over an istream... So, I decided to do it like in the old school parsing by hand... We probably can create a wrapper that matches with the STL bidirectional iterator classes and move over the file as a simple std::string...
Using boost is not an option because is a big library and the implementation over istreams is probably reading all the data into a memory buffer and then apply the operations over there (with big data set we have the same problem)..


Things to improve
-----------------------
- Parser using regular expressions or another lexer / parser.
- Check for possible overflows (when summing integers for example).
- Improve the design and how the files are organized. If we have more specifications defined it will be much more easier.

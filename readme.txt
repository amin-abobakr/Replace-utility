in the first you must put your files.txt in the project folder
supported format is .txt files only

this code performs the replace utily 
1-this code handle strings start with “-” or “--“
2- replace word whether written in upper case or lower case 
TEST CASES Applied on this code 
1-Example 1:
replace -i Howdy Hello -- file1.txt file2.txt file3.txt 
would replace all occurrences of “Howdy” with “Hello” in the files specified.
Because the “-i” option is specified, occurrences of “howdy”, “HOwdy”,
“HOWDY”, and so on would also be replaced.
this test case passed
 OPT supported 
 _i
 -f
 -l
 -b 
 -i -b 
 -f -l -b 
 note the arrangement of OPTs is nt case passed

replace -b -f Bill William -- file1.txt file2.txt
would replace the first occurrence of “Bill” with “William” in the two files
specified. It would also create a backup copy of the two files before performing
the replace.
this test case passed.
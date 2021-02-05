### Team Details :
Anant Jain - 2019MCS2557
Meraj Ahmed - 2019MCS2565
Priyadarshi Hitesh - 2019MCS2569

### How to execute :
1.  First run install.sh that will clone the zip file of assignment 1 containing everything.
2.  Run compile.sh so that it produces all the exectables needed to run the whole program
3.  For running the main MCS192565.sh there are two options:
    1st -> ./MCS192565.sh arg1 arg2 arg3 arg4 where arg1 is filename, arg2 is support threshold in(%), arg3 can be '-apriori' or '-fptree', arg4 is filename in which output would be written.
    2nd -> ./MCS192565.sh arg1 arg2 where arg1 is filename and arg2 is '-plot' which specifies plotting needs to be done.

### Files Bundled:

#### apriori.cpp: 
It contains code written to generate frequent itemset given file and threshold using Apriori Algorithm.
#### fptree.cpp: 
It contains code written to generate frequent itemset given file and threshold using FP Tree Algorithm.
#### install.sh: 
It will help clone the github repository of the assignment
#### compile.sh: 
This shell script compile both cpp program mentioned above and outputs their executables.
#### MCS192565.sh: 
This shell script will provide options to run the apiori or fptree algorithm given file and threshold, it also provides options to make plots of time taken by different algorithm in order to compare it.
#### plotting.py: 
It contains code for making plots depicting comparison between apriori and fptree algorithms according to parameter time taken with respect to different threshold in this list [1,5,10,25,50,90].



# Apriori and FP Tree Algorithm

We have implement *apriori algorithm* and *FP tree* for given input file and support value.

The argument of the program :-
 - Input file contain all the transaction where each transaction contain certain items
 - Support Value (Here, it is given in percentage )
 - Ouput file (Obtained Itemsets will be written in this file in desired order)


# Apriori Algorithm
Apriori Algorithm (popular algorithm in data mining) is used to get frequent itemset from given list of transaction of items and given value of support.

### The functions used in this program
#### getItemval
Given transaction string from the input file , it return vector of items in the integer form.

#### getIntersectlen 
Given two vectors , it return the last index for which both vectors have same type of values. It is used during joining step in the process of candiate generation.

#### getCountOfEachItemSet 
Given the name of input file , list of itemsets , map and count of different types of items, this function return the count of each type of itemset existing in the transactions of the input file. By this , We are able to get our k-frequent set 
from given (k-candiate set).


We have run our program on the "webdocs.dat" file given in the assignment. Following are our results :-

Note :- Webdocs.dat contain 1692082 transactions and 1692082 different items

----------------------------------------------------------------------------|
Support (in %)   |  Rows obtained   | Time taken (in ns)                    |
----------------------------------------------------------------------------|
   90            |        0         |   130491 ms (2 min 10 sec)            |
   50            |       10         |   308366 ms (5 min 8 sec)             |
   25            |      413         |   862614 ms (14 min 23 sec)           |
   20            |     1616         |  2048637 ms (34 min 9 sec)            |
   15            |    10388         |  9516311 ms (2 hr 38 min 36 sec)      |
----------------------------------------------------------------------------|

We have tried for support of 10, but it is taking lots of time (more than 10 hour).So That case cannot be run properly.
So, We cannot able to run case of support value of 5 and 1



# FP Tree Algorithm
FP tree algorithm is efficient and scalable method for mining the complete set of frequency.

#### fpTree.cpp:
Fp-tree is implemented in fpTree.cpp

#### fpGrowth: 
This is the important recursive function implementing frequent pattern mining using fp-tree without candidate set generations.

#### constructCondFpTree:
It takes conditional base tree and a item and generate new conditional tree.

#### generateCombinations:
It takes list of items and generate all combinations of items in list using backtracking.

#### backTrack:
It is recursive function to generate all combination given list.

#### isSinglePath:
It takes an fp-tree and return true if contains of only single path, false otherwise.

#### update_frequency:
It is a helper function used to update the frequency of each items in 1st database scan.

#### split:
It takes 1 line of transaction string and convert it to vector of integer.

#### asciiCmp:
It boolean comparator for sorting final output in ascii order.

#### sortByFrequency:
It takes frequncey of items and sort in descending frequency

#### printFrequent:
It takes one frequent item set in ascending ascii order and print on the terminal. Each item is space separated and each frequent item sets is in new line


#### FpNode (class):
It is node class representing single node.
Every node contains:
 - item
 - count of item
 - children pointer
 - parent pointer

#### FpTree (class):
It is tree class representing fp-tree
It contains:
 - root node
 - header table: It contains list of pointer to same value items
 - freq items: It contains frequency of each item currently present in the fp-tree
 - insert(): It ia function to insert a path in the tree starting from root


#### FP Tree Time
Support(%) : Time taken
90 % => 66 sec
50 % => 4 min 30 sec
25 % => 10 min 12 sec
20 % => 24 min
15 % => 120 min
10 % => 213 min
5 % => more than 360 min
1 % => more than 8 hr


#### Observation for part 2(b):
- We have implemented FP tree and Apriori algorithm in C++. 
- Running time of FP Tree in Apriori differs mainly because of no. of database scan.
- We have observed that for larger threshold percent, our algorithm terminates quickly beacause larger threshold means larger threshold. So we will be getting very small set of frequent set items.
- For larger threshold Apriory algorithm also terminates in almost after 2 databse scan. So we got almost near run time for threshold like 90% and 50%
- As we decrease the threshold the running time differs in several minutes like we got around 4 minutes difference for 25 % threshold.
- If we decrese threshold more, like at 15 %, we are getting more than 10,000 frequent item sets. There is major increase in runtime from 25 % to 15 %. 
FP tree runs in around 2 hr, while apriori takes around 2 hr 45 min. There is almost 45 minutes difference.
- At vey low threshold like 1 % and 5 %, Fp tree and Apriori both takes much time. I have reported minimum it takes more than 6 hr on 5 % and  even much time on 1%.

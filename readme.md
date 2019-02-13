Input is expected in the form of a file consisting of transactions where each transaction is on a new line and each transaction consists a list of items separated with commas

To run compile main.cpp and run it with 2 command line parameter i.e. the path to file and minimum support threshold percentage.

Example usage :

g++ main.cpp -o fptree

fptree datasets/groceries.csv 20

The output would be generated in a file named frequent_itemsets.txt

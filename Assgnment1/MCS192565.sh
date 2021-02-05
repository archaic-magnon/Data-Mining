#!/bin/bash
#Here $1 specifies input file here, $2 specifies threshold for algos and plot for plotting
#$3 specifies which algo to use and $4 specifies out file in which results will be written
strvar=.txt
filename=$4$strvar
if [ "$3" == "-fptree" ]; then
	./fpOut $1 $2 $filename > $filename
elif [ "$3" == "-apriori" ]; then
 	./aprOut $1 $2 $filename > $filename
elif [ "$2" == "-plot" ]; then
	python3 plotting.py $1
else
	echo "Please provide the valid argument"
fi

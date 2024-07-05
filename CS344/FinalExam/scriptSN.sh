#!/bin/bash
#Scott Nicholson Final 
count=$#
selection=""
if [ "$count" -gt 0 ] 
then
	selection=$1
else
	echo "Usage ./scriptSN.sh [c,d]"
fi

if [ "$selection" = "c" ] 
then
	file=""
	read -r -p "Please enter a text file: " file
	if [ "$file" == "" ] || [ ! -f "$file" ]
	then
		echo "Please enter a valid text file"
		exit
	fi

	cat $file
	echo
elif [ "$selection" = "d" ]
then
	directory=""
	read -r -p "Please enter a directory: " directory 
	if [ "$directory" == "" ] || [ ! -d "$directory" ]
	then
		echo "Please enter a valid directory"
		exit
	fi

	ls $directory
	echo
else
	echo "The only parameters accepted are d or c"
fi

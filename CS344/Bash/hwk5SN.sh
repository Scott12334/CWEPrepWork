#!/bin/bash
#Scott Nicholson HWK 5 14 April 2024
count=$#
choice=""
if [ "$count" -gt 0 ]; then
	choice=$1
fi

while [ ! "$choice" = "Q" ] && [ ! "$choice" = "q" ]; do
	if [ "$choice" = "" ];then
		echo "1. List all files in current directory"
		echo "2. List all files in a specified directory"
		echo "3. Display all current processes on the system"
		echo "4. Display all the global enviroment variables"
		echo "Enter Q to quit"
		read -r -p "Make a selection please: " choice
	fi
	
	if [ "$choice" = "1" ];then
		ls
		echo
	fi

	if [ "$choice" = "2" ];then
		directory=""
		read -r -p "Please enter the directory: " directory
		if [ "$directory" = "" ] || [ ! -d "$directory" ];then
			echo $directory
			echo "Please enter a valid directory"
			continue
		fi
		ls $directory
		echo
	fi

	if [ "$choice" = "3" ];then
		ps aux
		echo
	fi

	if [ "$choice" = "4" ];then
		env
		echo
	fi
	if [ "$choice" = "Q" ];then
		break
	fi
	choice=""
done

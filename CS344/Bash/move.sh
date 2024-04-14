#!/bin/sh

if [ ! $# -eq 2 ]; then
	echo "Usage ./move.sh File Directory"
	exit
fi

if [ ! -f $1 ]; then
	echo "File does not exist"
	exit
fi

if [ ! -d $2 ]; then
	echo "Directory does not exist"
	exit
fi
mv $1 $2
echo "Good jobe"

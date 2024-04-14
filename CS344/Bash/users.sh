#!/bin/bash
clear
choice=""
while [ ! "$choice" = "Q" ] && [ ! "$choice" = "q" ]; do
  echo "L)ist users"
  echo "C)ount users"
  echo "Q)uit script"
  read -r -p "Make a selection please: " choice

  if [ "$choice" = "" ]; then
     echo "You did not make a selection"
     echo
     continue
  fi 
  
  if [ "$choice" = "L" ] || [ "$choice" = "l" ]; then
     ls -l /home | grep "^d" | rev | cut -d' ' -f1 | rev
     echo
     continue
  fi

  if [ "$choice" = "C" ] || [ "$choice" = "c" ]; then
     count=$(ls -l /home | grep -c "^d")
     echo "Number of users: $count"
     echo
     continue
  fi

  if [ "$choice" = "Q" ] || [ "$choice" = "q" ]; then
     echo "Bye"
     continue
  fi

  echo "$choice is invalid"
  echo
done

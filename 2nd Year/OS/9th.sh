#!/bin/bash

# Prompt user for input
read -p "Enter the string to search: " search_string
read -p "Enter the filename: " filename

# Check if file exists
if [ ! -f "$filename" ]; then
    echo "Error: File does not exist."
    exit 1
fi

# Search for the string in the file using grep
grep -n "$search_string" "$filename"

# Check if grep found a match
if [ $? -ne 0 ]; then
    echo "No matches found for '$search_string' in $filename."
else
    echo "Search complete."
fi

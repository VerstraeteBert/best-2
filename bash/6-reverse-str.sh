#!/bin/bash -

function reverse_str() {
    str=$1;
    res=""

    for (( i=${#str} - 1; i >= 0; i--)); do
        res="$res${str:i:1}"
    done

    echo "$res";
}

[[ -z $1 ]] && { echo "Geef een string mee"; exit 1; }

echo $(reverse_str $1);
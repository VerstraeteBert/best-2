#!/bin/bash -

num=$1

[[ -z $1 ]] && { echo "FOUT: geen input" 1>&2; exit 1; }

[[ $num =~ ^[0-9]+$ ]] || { echo "FOUT: geef een positief geheel getal in" 1>&2; exit 1; }

nums=( 1000 900 500 400 100 90 50 40 10 9 5 4 1 )
lets=( "M" "CM" "D" "CD" "C" "XC" "L" "XL" "X" "IX" "V" "IV" "I" )

declare res;

for ((i=0; $i < ${#nums[@]}; i++)); do
    while (( num >= ${nums[$i]} )); do
        res="${res}${lets[$i]}"
        ((num = num - ${nums[$i]}));
    done
done

echo "$res";
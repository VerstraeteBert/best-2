#!/usr/bin/env bash -

function calc_fact() {
    num=$1
    factorial=1

    while (( num > 0 ))
    do
        (( factorial = factorial * num ))
        (( num-- ))
    done

    echo $factorial
};

[[ -z $1 ]] && { echo "Geef een pos geheel getal in "; exit 1; }

[[ ! $1 =~ ^[0-9]+$ ]] && { echo "Geef een pos geheel getal in "; exit 1; }

echo $(calc_fact $1)

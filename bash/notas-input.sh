#!/bin/bash -

# (( )) -> controleren op ints
(( $# != 1 )) && { ECHO Fout 1>&2; exit 1; }

# [[ ]] -> controleren op strings
# idem als in perl =~ -> regex eval
[[ $1 =~ ^[0-9]+$ ]] || { echo "$1 stelt geen getal voor!" ; exit 1; }
# note: commando's groeperen met () kan ook, echter worden de commandos in een subshell uitgevoerd


# brace expansion geevalueerd voor de variable subs
# -> eval gebruiken
# -> zal eerst volledige expansie / subs proces doorlopen
# -> omzetten naar string en uitvoeren
eval echo {0..$1} | xargs -n1
# xargs om stdout te manipuleren -> -n1 -> elke output (gescheiden door spatie) op aparte lijn

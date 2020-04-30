#!/usr/local/bin/bash -

N=5

function to_decimal() {
    IFS=. read  a b <<< "$1"
    echo $((a*10+b))
}

declare -a top_pid=(0 0 0 0 0)
declare -a top_cpu=(-1 -1 -1 -1 -1)

function insert_idx() {
    local insert_pos=$1
    local cpu=$2
    local pid=$3

    local j
    for (( j = N - 1; j > insert_pos; j-- ))
    do
        top_cpu[$j]=${top_cpu[$((j-1))]}
        top_pid[$j]=${top_pid[$((j-1))]}
    done

    top_cpu[$insert_pos]=$cpu
    top_pid[$insert_pos]=$pid
}

function insert() {
    local cpu=$1
    local pid=$2

    local i=$((N-1))

    local intval=$(to_decimal $1)
    while (( intval > top_cpu[i] && i >= 0 ))
    do
        ((i--))
    done

    ((i++))

    (( i <= 4 )) && insert_idx $i $intval $pid
}

exec 3< <(ps aux)

#skip header line
read <& 3

# read in all pid & cpu, don't include own process
while IFS=" " read user pid cpu rest <& 3
do
    # $$ holds own process id
    if (( pid != $$ ))
    then
        insert $cpu $pid
    fi
done

exec 3<&-

for (( i=0; i < N; i++ ))
do
    echo "${top_pid[$i]} ${top_cpu[$i]}"
done


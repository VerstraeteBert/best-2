#! /usr/local/bin/bash -

# params
# -s -> success (print overview of hosts that received a lease per date)
# -r -> rejected (print number of rejections of lease)
# -n -> night (print hosts that received a renewal during night time)
# -f -> set file to parse

function print_usage() {
    echo "USAGE: logfilter [-n] [-r] [-n] [-f filename]"
    echo -e "\t-s (success) print overview of hosts per date that received a lease"
    echo -e "\t-r (rejected) prints number of lease rejections"
    echo -e "\t-n (night) prints number of renewals during night time"
    echo -e "\t-f (filename) filename to parse"
    echo -e "\tif no arguments are supplied, -r, -s, -n are implied"
}

s_set=0
r_set=0
n_set=0

filename="fixtures/dhcp.log"

if [[ "$#" -eq 0 ]]; then
    s_set=1
    r_set=1
    n_set=1
else
    params=("$@")
    for (( i = 0; i < $# ; i++ )); do
        if [[ "${params[$i]}" == "-n" ]];
        then
            n_set=1
        elif [[ "${params[$i]}" == "-r" ]];
        then
            r_set=1
        elif [[ "${params[$i]}" == "-s" ]];
        then
            s_set=1
        elif [[ "${params[$i]}" == "-f" ]];
        then
            ((i++))
            [[ ! -f "${params[$i]}" || ! -r "${params[$i]}" ]] && { echo "File doesn't exist or is not readable"; exit 1; }
            filename=${params[$i]};
        else
            print_usage && exit 1;
        fi
    done
fi

exec 3< "$filename"

# skip until header line
IFS=","
while read id rest && [[ "$id" != "ID" ]]
do
    : # do nothing
done <& 3
#skip header line
read <& 3

num_refusals=0
declare -A hosts_night
declare -A mac_denied

function is_night() {
    local timestamp=$1
    IFS=":"
    read hour rest <<< "$timestamp"
    [ "$hour" -gt "19" ] || [ "$hour" -lt "6" ] && { echo "1"; }
}

curr_leasedate=""

function print_success_lease() {
    local date=$1
    local timestamp=$2
    local hostname=$3
    local mac=$4

    if [[ "$date" != "$curr_leasedate" ]]
    then
        curr_leasedate="$date"
        echo "- $date -"
    fi
    echo "$timestamp: $hostname through $mac"
}

#id 10 -> lease granted
#id 15 -> lease rejected
#id 11 -> lease renewed
(( s_set == 1 )) && echo "---- Successful leases per date ---"

while IFS="," read id date timestamp descr ip hostn mac rest
do
    (( n_set == 1 && id == 11 )) && [[ $(is_night "$timestamp") == "1" ]] && { hosts_night["$hostn"]=undef; }
    (( r_set == 1 && id == 15 )) && { mac_denied["$mac"]=undef; }
    (( s_set == 1 && id == 10 )) && { print_success_lease "$date" "$timestamp" "$hostn" "$mac"; }
done <& 3

exec 3<&-


if (( r_set == 1 ))
then
    echo "--- Number of distinct hosts lease rejection ---"
    echo ${#mac_denied[@]}
fi

if (( n_set == 1 ))
then
    echo "--- Hosts with night renewals ---"
    for host in ${!hosts_night[@]}
    do
        echo "$host"
    done
fi

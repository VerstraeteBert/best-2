#!/usr/local/bin/bash -

declare -A ppid_idx_map
declare -A pid_name_map

function print_tree_level() {
    local ppid=$1
    local level=$2

    if [[ ! -v "ppid_idx_map['$ppid']" ]]
    then
        return 0
    fi

    IFS=','
    for pid in ${ppid_idx_map["$ppid"]}
    do
        for (( i = 0; i < level ; i++ ))
        do
            printf '\t'
        done

        printf 'PPID:%s PID:%s Name:%s\n' "$ppid" "$pid" "${pid_name_map[$pid]}"

        print_tree_level "$pid" "$((level + 1))"
    done
}

exec 3< <(ps -ef)

#skip header
read <& 3

while read uid pid ppid c stime tty ctime name <& 3
do
    pid_name_map["$pid"]=$name
    if [[ ! -v "ppid_idx_map['$ppid']" ]]
    then
        ppid_idx_map["$ppid"]="$pid"
    else
        ppid_idx_map["$ppid"]="${ppid_idx_map["$ppid"]},$pid"
    fi
done

exec 3<&-

print_tree_level "0" "0"

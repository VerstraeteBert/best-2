#!/usr/local/bin/bash -

declare -A ppid_idx_map
declare -a children_arr
declare -A pid_name_map

function print_tree_level() {
    local ppid=$1
    local level=$2

    if [[ ! -v "ppid_idx_map['$ppid']" ]]
    then
        return 0
    fi

    idx=${ppid_idx_map["$ppid"]}
    IFS=','
    for pid in ${children_arr["$idx"]}
    do
        for (( i = 0; i < level ; i++ ))
        do
            printf '\t'
        done

        printf '%s\n' "${pid_name_map[$pid]}"

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
        ppid_idx_map["$ppid"]=${#children_arr[@]}
        children_arr[${ppid_idx_map["$ppid"]}]="$pid"
    else
        idx=${ppid_idx_map["$ppid"]}
        children_arr[$idx]="${children_arr[$idx]},$pid"
    fi
done

print_tree_level "0" "0"

exec 3<&-

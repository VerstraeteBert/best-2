#!/usr/bin/env bash -

input_arr=()
# key val map
declare -A occur_arr
declare -A group_arr

IFS=:
while read -a input_arr
do
    group_id=${input_arr[3]}

    (( occur_arr["$group_id"]++ ))
done < "/etc/passwd"

while read -a input_arr
do
    gid=${input_arr[2]}
    gn=${input_arr[0]}
    group_arr["$gid"]=$gn
done < "/etc/group"

for key in ${!occur_arr[@]}
do
    echo "${group_arr[$key]}: ${occur_arr[$key]}"
done

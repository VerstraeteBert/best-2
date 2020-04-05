#!/usr/bin/env bash -

file="/etc/passwd"
input_arr=()
# key val map
declare -A res_arr

while IFS=: read -a input_arr
do
    [[ -z ${input_arr[3]} ]] && { continue; }

    group_id=${input_arr[3]}

    (( res_arr["$group_id"]++ ))
done < "$file"

for key in ${!res_arr[@]}
do
    echo "$key: ${res_arr[$key]}"
done

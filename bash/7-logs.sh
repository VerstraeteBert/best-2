#!/bin/bash -

declare -A uri_get_map

init_dir=$(pwd)
count_processed=0

temp_dir=$(mktemp -d "/tmp/logs.XXXXXX")

trap 'cleanup_and_print_res' INT HUP

# $1 -> filename to parse
function count_uri_get() {
    local filename=$1

    while read -r date timestamp ip method uri rest
    do
        if [[ "$method" == GET &&  "$uri" =~ \/ ]]
        then
            (( uri_get_map[$uri]++ ))
        fi
        (( count_processed++ ))
    done < "$filename"
}

function cleanup_and_print_res() {
    total_count=$(cat *.log | wc -l)

    $(cd "$init_dir")

    $(rm -rf "$temp_dir")

    for key in ${!uri_get_map[@]}
    do
        echo "$key ${uri_get_map[$key]}"
    done | sort -k 2rn | head -n 10

    pct=$(bc -l <<< "$count_processed / $total_count * 100")

    printf "\nPercent processed: %.1f%%\n" "$pct"

    exit 0;
}

temp_dir=$(mktemp -d "/tmp/logs.XXXXXX")

cp 'iis-logs.zip' "$temp_dir/iis-logs.zip"

cd "$temp_dir"

unzip -q "$init_dir/iis-logs.zip"

files="*.log"
for filename in $files; do
    count_uri_get "$filename"
done

cleanup_and_print_res

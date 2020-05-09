#!/usr/local/bin/bash -

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


    printf "Total=%d Processed=%d%\n" "$total_count" "$count_processed"
    processed_mult=$(( count_processed * 100 ))
    pct_dec=$(( processed_mult / total_count ))
    pct_frac=$(( (processed_mult % total_count) / 10 ))
    printf "\nPercent processed: %d.%d%\n" "$pct_dec" "$pct_frac"

    exit 0;
}

temp_dir=$(mktemp -d "/tmp/logs.XXXXXX")

cp 'fixtures/iis-logs.zip' "$temp_dir/iis-logs.zip"

cd "$temp_dir"

unzip -q "$init_dir/iis-logs.zip"

files="*.log"
for filename in $files; do
    count_uri_get "$filename"
done

cleanup_and_print_res

#!/usr/local/bin/bash -

exec 3<fixtures/callisto.log
exec 4<fixtures/ganymedes.log
exec 5>merged.log

IFS=" "
end_a=0
end_b=0

read -u 3 -r af1 af2 af3 adate arest || end_a=1;
read -u 4 -r bf1 bf2 bf3 bdate brest || end_b=1;

while (( end_a != 1 && end_b != 1 ))
do
    if [[ "$adate" > "$bdate" ]]
    then
        echo "Callisto ${af1} ${af2} ${af3} ${adate} ${arest}" >& 5
        read -u 3 -r af1 af2 af3 adate arest || end_a=1;
    else
        echo "Ganymedes ${bf1} ${bf2} ${bf3} ${bdate} ${brest}" >& 5
        read -u 4 -r bf1 bf2 bf3 bdate brest || end_b=1;
    fi
done

while (( end_a != 1 ))
do
   echo "Callisto ${af1} ${af2} ${af3} ${adate} ${arest}" >& 5
   read -u 3 -r af1 af2 af3 adate arest || end_a=1;
done

while (( end_b != 1 ))
do
    echo "Ganymedes ${bf1} ${bf2} ${bf3} ${bdate} ${brest}" >& 5
    read -u 4 -r bf1 bf2 bf3 bdate brest || end_b=1;
done

exec 3<&-
exec 4<&-
exec 5>&-

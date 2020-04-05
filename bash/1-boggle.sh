#!/bin/bash -

array=( "NTOKEZ" "CDEPAM" "LUNPET" "MOARSI" "DONSTE" "HIEFES"
        "RESHCA" "BQMJOD" "TIALNB" "GYNJEU" "ZAEDNV" "AOIEAW"
        "HSIREN" "GENVIT" "WEGLUR" "RIAFXK"
      );

# opdracht -> eerst dobbelstenen rollen en daarna willekeurig in raster plaatsen
for (( i=0; i<${#array[@]}; i++ )); do
    # genereer random getal van 0-5 -> index v/ letter
    # $() -> command substitutie
    ran_pos=$(shuf -i 0-5 -n 1)
    # ${string:position:length}
    array[$i]=${array[$i]:ran_pos:1};
done

shuf -e ${array[@]} | xargs -n4

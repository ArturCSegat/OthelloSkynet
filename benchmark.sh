#!/bin/sh

o_counter=0
x_counter=0

o="o"
x="x"
i=0
while [ "$i" -lt $1 ]; do
    output=$(./build/Skynet)
    last_line=$(echo "$output" | tail -n 1)
    last_char=$(echo "$last_line" | rev | cut -c 1 | rev)

    echo "last char: $last_char"
    if [ "$last_char" = "$o" ]; then
        o_counter=$((o_counter + 1))
    fi
    if [ "$last_char" = "$x" ]; then
        x_counter=$((x_counter + 1))
    fi

    echo "x: $x_counter"
    echo "o: $o_counter"
    i=$((i+1))
done

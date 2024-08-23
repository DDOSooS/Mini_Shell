#!/bin/bash
#dlmlmdlmd

echo "enter the first number"
read num1

echo "enter the second number"
read num2

if [ $num1 -eq $num2 ]; then
        echo "the numbers match"
else
        echo "the number do NOT match"
fi

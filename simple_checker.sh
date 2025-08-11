#!/bin/bash

./hsh < ls_empty_path.bash > out.txt 2>&1

echo "---- out.txt ----"
cat out.txt || true
echo "-----------------"

echo "./hsh: 1: ls: not found" > expected.txt

echo "---- expected.txt ----"
cat expected.txt
echo "----------------------"

echo "---- diff (expected vs out) ----"
diff -u expected.txt out.txt || true
echo "--------------------------------"

echo "Exit status of the shell run: $?"

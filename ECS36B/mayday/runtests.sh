#!/bin/bash
for i in {1..5}
do
    echo ======= test$i ======
  ./testmayday$i.sh > a.txt
  diff a.txt testmayday$i.out
done

rm -f a.txt
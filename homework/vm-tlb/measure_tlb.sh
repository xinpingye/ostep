#!/bin/bash

for i in $(seq 1 40);
do
    nums=$(taskset -c 1,1 ./tlb  $i 10)
    echo $i $nums >> ./example.dat
    sleep 1
done

pl -prefab lines data=example.dat x=1 y=2 -svg -o out.svg



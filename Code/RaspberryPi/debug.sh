#!/bin/bash

for i in {1..10}
do
    echo "$i"
    ./zeppelin &
    sleep 1
    kill -SIGINT $!
    sleep 15
    echo
done
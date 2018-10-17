#!/bin/bash

ene=200
root -l -q "test.cpp($ene)" > memo

for ((ene=500;ene<=19500;))
do
    root -l -q "test.cpp($ene)" >> memo
    ene=$(( ene + 500 ))
done

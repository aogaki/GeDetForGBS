#!/bin/bash

nEvents=1000000

ene=200
echo "$ene keV"

echo "/GD/Primary/K $ene" > tmp.mac
echo "/run/beamOn $nEvents" >> tmp.mac

./example tmp.mac
hadd -f "resultEne$ene".root result_t*

echo "/GD/Primary/K $ene" > tmp.mac
echo "/GD/Primary/RMS 0." >> tmp.mac
echo "/run/beamOn $nEvents" >> tmp.mac

./example tmp.mac
hadd -f "resultMonoEne$ene".root result_t*


for ((ene=500;ene<=19500;))
do
    echo "$ene keV"

    echo "/GD/Primary/K $ene" > tmp.mac
    echo "/run/beamOn $nEvents" >> tmp.mac

    ./example tmp.mac
    hadd -f "resultEne$ene".root result_t*

    echo "/GD/Primary/K $ene" > tmp.mac
    echo "/GD/Primary/RMS 0." >> tmp.mac
    echo "/run/beamOn $nEvents" >> tmp.mac

    ./example tmp.mac
    hadd -f "resultMonoEne$ene".root result_t*

    ene=$(( ene + 500 ))
    
done

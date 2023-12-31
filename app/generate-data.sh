#!/bin/bash

duration=15
size=1540
cores=(1 2 4 10 14)

for n in "${cores[@]}"
do
  make simulator=Naive duration=${duration} size=${size} cores=$n out=true run
  make simulator=BarnesHut duration=${duration} size=${size} cores=$n out=true run
done
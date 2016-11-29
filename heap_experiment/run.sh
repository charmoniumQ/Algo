#!/bin/sh

javac TimeHeap.java BinaryHeap.java ThreeHeap.java MyPQ.java
touch binary2_time.csv ternary2_time.csv fibonacci5_time.csv
rm binary2_time.csv ternary2_time.csv fibonacci5_time.csv
java TimeHeap 1 > binary2_time.csv
java TimeHeap 2 > ternary2_time.csv
java TimeHeap 3 > fibonacci5_time.csv

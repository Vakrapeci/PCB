#!/bin/bash

echo "This is a flood test - it will turn the LED on and off for 500 times"

for (( i=0; i<500; i++))
do
    echo '1' > /dev/ttyACM0
    echo '0' > /dev/ttyACM0
done

echo "Test ended"

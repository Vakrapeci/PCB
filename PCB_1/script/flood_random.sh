#!/bin/bash

echo "This is a flood test to test what happens if any other characer is sent (not 0 or 1)."
echo "It will send out 500 random hexadecimal character."

for (( i=0; i<500; i++))
do
    echo $RANDOM | md5sum | head -c 1 > /dev/ttyACM0
done
echo '0' > /dev/ttyACM0

echo "Test ended"

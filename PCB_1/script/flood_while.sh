#!/bin/bash

echo "This is an infinite flood-test"
echo "  to interrupt it press ctrl+C"

while [ 1 ]
do
    echo '1' > /dev/ttyACM0
    echo '0' > /dev/ttyACM0
done

echo "Test ended"

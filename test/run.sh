#!/bin/bash

usage(){
cat << EOF
usage:
    ./run.sh [file_name]
EOF
}

echo -n "Current time : "
date +"%Y-%m-%d %H:%M:%S"
if [[ -z $1 ]]; then
    usage
    exit 1
fi
arduino-cli compile --fqbn arduino:avr:mega "$1" || exit 1
echo "Compilation successful. Uploading..."

port="/dev/ttyACM0"
if [[ -n $2 ]]; then
    port=$2
fi

arduino-cli upload -p $port --fqbn arduino:avr:mega "$1"



#!/bin/bash
./Binaries/run $@ & ./Binaries/view $(pgrep -o -x run)


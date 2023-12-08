#!/bin/bash

Yell='\033[1;33m'
NC='\033[0m' # No Color

head="parte1/test_cases/"
for i in `ls parte1/test_cases`
do
    caso="$head$i"
    echo -e "${Yell}TESTING:${NC} $caso\n"
    python parte1/CSPParking.py $caso
done

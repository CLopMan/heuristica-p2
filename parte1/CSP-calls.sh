#!/bin/bash

Yell='\033[1;33m'
NC='\033[0m' # No Color

head="test_cases/"
for i in `ls test_cases`
do
    caso="$head$i"
    echo -e "${Yell}[TESTING]:${NC} $caso\n"
    python CSPParking.py $caso
done

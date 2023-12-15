#!/bin/bash

Yell='\033[1;33m'
Green='\033[0;32m'
NC='\033[0m' # No Color

head="ASTAR-tests/"
for i in `ls ASTAR-tests | grep '\.csv$'`
do
    caso="$head$i"
    echo -e "${Yell}[TESTING]:${NC} $caso\n"
    python ASTARTraslados.py $caso "1"
    echo -e "${Green}[FINISH]:${NC} $caso-"1"\n"
    python ASTARTraslados.py $caso "2"
    echo -e "${Green}[FINISH]:${NC} $caso-"2"\n"
    python ASTARTraslados.py $caso "3"
    echo -e "${Green}[FINISH]:${NC} $caso-"3"\n"
    python ASTARTraslados.py $caso "4"
    echo -e "${Green}[FINISH]:${NC} $caso-"4"\n"
    python ASTARTraslados.py $caso "5"
    echo -e "${Green}[FINISH]:${NC} $caso-"5"\n"
done

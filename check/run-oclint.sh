#!/bin/bash

# gets script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# defines settings
PATH_OCLINT=$1
OPTIONS=$(cat $DIR/options.conf)
COMPILER_OPTIONS='-std=c++11 -Wall -g -Iinclude -Iexternal/googletest/googletest/include -Itest -c'

# generates a list of files
FILES=$(find $DIR/../src/ $DIR/../test/ -type f -name \*.cc -print)

# runs oclint
$PATH_OCLINT $OPTIONS $FILES -- $COMPILER_OPTIONS

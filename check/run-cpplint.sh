#!/bin/bash

# gets script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# defines settings
PATH_CPPLINT="../external/styleguide/cpplint/cpplint.py"
OPTIONS="--root="$DIR"/../include/ --extensions=h,cc --linelength=80 --filter=-legal/copyright"

# generates a list of files
DIRS_SEARCH="$DIR/../include/ $DIR/../src/ $DIR/../test/"
FILES=$(find $DIRS_SEARCH -type f \( -name "*.cc" -o -name "*.h" \) -print)

# runs cpplint
$DIR/$PATH_CPPLINT $OPTIONS $FILES


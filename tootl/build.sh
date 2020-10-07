#!/bin/bash
if make ; then
    ./build/linux/x86_64/a.out
else
    echo "Build Failed"
fi

#!/bin/sh

make_target() {
    make TARGET=${1} clean 
    make TARGET=${1} 
}

make_target WRAITH32_35A
make_target MAVERICK
make_target KISS24A
make_target KISS24A_KISSBL

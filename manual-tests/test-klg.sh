#!/bin/bash
# 
# version:20080313-1 
#
# Script to generate and test common resampling
# use cases. The output files need to be verified
# manually.
#
# ----------------------------------------------------------------------
# File: ecasound/manual-tests/test-klg.sh
# License: GPL (see ecasound/{AUTHORS,COPYING})
# ----------------------------------------------------------------------


ECASOUND=../ecasound/ecasound_debug
#ECASOUND=ecasound

# whether to skip md5sum checks
SKIP_MD5SUM=0

. test-common-sh

check_ecabin

set -x

# control amplify with klg
#   0.0  -> 5.5:   0%   -> 100%
#   5.5  -> 10.5:  100% -> 20%
#   10.5 -> 20.0:  20%  -> 80%
#   20.0 -> 30.0:  80%  -> 10%
#
$ECASOUND -q -f:16,1,44100 -i tone,sine,440,30 -o klg-dst.wav -ea:100 -klg:1,0,100,5,0,0,5.5,1,10.5,0.2,20.0,0.8,30,0.1 -x  || error_exit
check_md5sum klg-dst.wav e7c1a4d352423eb9c40f184274546c8e

echo "Test run succesful."
exit 0
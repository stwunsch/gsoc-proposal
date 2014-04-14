#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/stefan/git/radsens/gr-radar/python
export PATH=/home/stefan/git/radsens/gr-radar/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/stefan/git/radsens/gr-radar/build/swig:$PYTHONPATH
/usr/bin/python2 /home/stefan/git/radsens/gr-radar/python/qa_simulator_cc.py 

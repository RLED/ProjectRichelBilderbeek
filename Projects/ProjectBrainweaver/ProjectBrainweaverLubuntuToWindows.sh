#!/bin/bash
myfile="i686-pc-mingw32-qmake"
mytarget="BrainweaverStudent"
myprofile=$mytarget".pro"
myexe=$mytarget".exe"

if [ ! -e $myprofile ]
then
  echo "Qt Creator project '$myprofile' not found"
  exit
fi

$myfile $myprofile

if [ ! -e Makefile ]
then
  echo "FAIL: $myfile $myprofile"
  exit
fi

make

if [ -e ./release/$mytarget ]
then
  echo $mytarget": SUCCESS"
else
  echo $mytarget": FAIL"
fi

#Cleaning up
#rm Makefile
#rm Makefile.*
#rm -r release
#rm -r debug

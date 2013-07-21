#!/bin/bash
#Script to check the status of compiles
myos='Lubuntu'
mytempfile='tmp'$myos'.txt'
if [ -e $mytempfile ]
then
  rm $mytempfile
fi


for mytest in BoostLexical_Cast BoostRegex BoostSignals BoostSignals2 BoostUblas BoostXpressive Cpp0x Cpp11 Qt4 Qt5 Qt Qwt World World64 Wt
do
myfolder='CppHello'$mytest'QtCreator'$myos
myscript=$myfolder'.sh'

if [ ! -d $myfolder ]
then
  echo "Folder '$myfolder' not found"
  continue
fi

cd $myfolder

if [ ! -e $myscript ]
then
  echo "Compile script '$myscript' not found"
  continue
fi

echo $mytest >> ../$mytempfile
./$myscript | egrep "SUCCESS|FAIL" >> ../$mytempfile

cd ..

done

cat $mytempfile | sed 'N;s/\n/: /'

rm $mytempfile

#!/bin/sh

# Verify that qmake if available
res=`which qmake > /dev/null`
if [ $? -ne 0 ]; then
  echo "qmake is not available. Please install qt/qt-dev to allow building."
  exit -1
fi

if [ $# -eq 1 ]; then
  if [ "X$1" = "Xall" ]; then
    make distclean
  fi
fi

# Making of Makefile
qmake

# Update of linguist files
lupdate *.pro
lrelease *.ts

# Building
make


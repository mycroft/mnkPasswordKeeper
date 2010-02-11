#!/bin/sh

if test $# -ne 1
then
    echo "Usage: $0 <version number>"
    exit 1
fi

cd $(dirname $0)

make clean
rm -f *.qm mnkPasswordKeeper

VERSION=$1

if test -d /tmp/mnkPasswordKeeper-$VERSION
then
    echo "Directory /tmp/mnkPasswordKeeper-$VERSION already exists."
    exit 1
fi

rm -f mnkPasswordKeeper-$VERSION.tar.bz2

mkdir /tmp/mnkPasswordKeeper-$VERSION
cp -R * /tmp/mnkPasswordKeeper-$VERSION
tar jcv -C /tmp -f mnkPasswordKeeper-$VERSION.tar.bz2 mnkPasswordKeeper-$VERSION
rm -fr /tmp/mnkPasswordKeeper-$VERSION


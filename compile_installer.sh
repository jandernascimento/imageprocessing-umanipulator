#!/bin/bash

#set -e

rm -rf temp || true

cp -r umanipulator temp

find ./temp/ -name ".svn" -exec rm -rf {} \; || true

cp -r $1/* ./temp/usr/lib/umanipulator/

dpkg --build temp

mv temp.deb umanipulator-rev$2.deb

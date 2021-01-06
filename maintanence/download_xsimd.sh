#!/bin/bash

VERSION=7.1.3
FILE=${VERSION}.tar.gz
URL=https://github.com/xtensor-stack/xsimd/archive/${FILE}
VALID="MD5 (${FILE}) = 1d831e7f6ca87aa75b09c6e88911c18f"

if [ ! -f "${FILE}" ]; then
	wget ${URL}
fi

CHECK=`md5 ${VERSION}.tar.gz`

echo "check: ${CHECK}"

if [ "${CHECK}" != "${VALID}" ]; then
	echo "Not valid download"
	exit -1
fi

tar -xf ${FILE}

mkdir -p ../inst
cp -r xsimd-${VERSION}/include ../inst/.
rm -rf xsimd-${VERSION}

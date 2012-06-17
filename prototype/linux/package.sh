#!/bin/bash

VERSION=0.47

package ()
{
	mkdir ./package/
	mkdir ./package/rsrc/

	cp ./axr_$1 ./package/
	cp ../core/hss/framework/UIFramework.hss ./package/rsrc/
	cp -r ../tests/ ./package/
	#cp -r ./tutorial/ ./package/

	tar -czf axr_${VERSION}_linux_$2.tar.gz ./package/*

	rm -rf ./package/
}

if [ -f ./axr_64 ]; then
	package "64" "x86-64"
fi

if [ -f ./axr_32 ]; then
	package "32" "x86"
fi


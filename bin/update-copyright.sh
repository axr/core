#!/bin/sh

find . -type f | egrep "\.(h|c|cpp|m|mm|txt)$" | xargs sed -i '' "s/COPYRIGHT: ©2011 - All Rights Reserved/COPYRIGHT: ©$(date +%Y) - All Rights Reserved/"

#!/bin/bash

IFS=%

for file in $@
do
	echo -e "checking $file\n"

	sed 's/\t/        /g' $file | grep -n '.\{81,\}'
done

unset IFS

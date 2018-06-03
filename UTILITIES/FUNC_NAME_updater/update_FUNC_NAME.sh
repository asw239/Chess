#!/bin/bash

#Internal Field Seperator breaks the formatting, disbale for now
IFS=%

for file in $@
do
	echo "Updating $file"

	#backup the file
	file_name=$(echo $file | rev | cut -d/ -f1 | rev)
	date_time=$(date +"%Y-%m-%d_%T")
	out_file_name=$file_name
	out_file_name+="_"
	out_file_name+=$date_time
	cp $file backup/$out_file_name

	line_cnt=$(wc -l $file | cut -f1 -d" ")

	for (( i=1 ; i<=$line_cnt ; i++ )) ;
	do
		txt_curr_ln=$(sed -n "${i}p" $file)

		#check if FUNC_NAME variable is declared in this scope
		#if not skip this function
		echo "$txt_curr_ln" | grep "*FUNC_NAME" > /dev/null
		if [ $? != 0 ]
		then
			continue
		fi

		#fnc_head begins two lines above *FUNC_NAME decl
		fnc_head_str_ln=$(($i-2))

		#store it in fnc_head array
		fnc_head=($(sed -n "${fnc_head_str_ln}p" $file))

		#check if function header is only one line long
		#if not keep adding the above line to fnc_head
		sed -n "${fnc_head_str_ln}p" $file | grep "(" > /dev/null
		while [ $? != 0 ]
		do
			fnc_head_str_ln=$(($fnc_head_str_ln-1))
			fnc_head+=($(sed -n "${fnc_head_str_ln}p" $file))
			sed -n "${fnc_head_str_ln}p" $file | grep "(" \
				> /dev/null
		done

		#fnc_head is inverted, reverse it
		x=0
		y=$((${#fnc_head[@]}-1))

		while [ $x -lt $y ]
		do
			tmp=${fnc_head[x]}
			fnc_head[x]=${fnc_head[y]}
			fnc_head[y]=$tmp
			x=$(($x+1))
			y=$(($y-1))
		done

		#remove leading tabs
		for (( j=0 ; j < ${#fnc_head[@]} ; j++ )) ;
		do
			fnc_head[j]=$(echo ${fnc_head[j]} | sed "s/^\t//")
		done

		#add open quotes at the start
		fnc_head[0]="\"${fnc_head[0]}"
		#add close quotes and semi colon at the end
		fnc_head_last=$((${#fnc_head[@]}-1))
		fnc_head[fnc_head_last]="${fnc_head[fnc_head_last]}\";"

		#function names that are multi lined, append \ to the end
		for (( j=0 ; j<$fnc_head_last ; j++ ))
		do
			fnc_head[j]="${fnc_head[j]} \\\\"
		done
	
		#begin file modification

		#find the start of FUNC_NAME string
		func_name_str=$(($i+1))

		#find the end of FUNC_NAME string
		func_name_end=$func_name_str
		sed -n "${func_name_end}p" $file | grep ")" > /dev/null
		while [ $? != 0 ]
		do
			func_name_end=$(($func_name_end+1))
			sed -n "${func_name_end}p" $file | grep ")" > /dev/null
		done
	
		#delete lines from func_name_str to func_name_end
		sed -i "$func_name_str,$func_name_end d" $file

		#append the updated name below the current line
		for (( j=0; j<${#fnc_head[@]} ; j++ )) ;
		do
			line_num=$(($i+1+$j))
			sed -i "$line_num i ${fnc_head[j]}" $file
		done
	done

	echo -e "Successfully updated $file\n"
done

#enable IFS
unset IFS

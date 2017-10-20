#! /bin/bash


random()
{
	index=0
	str=""
	t=`grep '((?=[\x21-\x7e]+)[^A-Za-z0-9])'`
	for i in {a..z}; do arr[index]=$i; index=`expr ${index} + 1`; done
	for i in {A..Z}; do arr[index]=$i; index=`expr ${index} + 1`; done
	for i in {$t}; do arr[index]=$i; index=`expr ${index} + 1`; done
	for i in {0..9}; do arr[index]=$i; index=`expr ${index} + 1`; done
	for i in {1..42}; do str="$str${arr[$RANDOM%$index]}"; done
	#echo $str
	s=`echo $str |cut -c1-42`
	echo ${s:0:6}-${s:12:6}-${s:18:6}-${s:24:6}-${s:30:6}-${s:36:6}
}

create()
(
	ip=`ifconfig | awk -F'[ ]+|:' '/inet addr/{if($4!~/^192.168|^172.16|^10|^127|^0/) print $4}'`
	read name
	a=`echo $name |wc -L`
	if [ $a -lt 6 ];then
		site=$name.$ip
	else
		site=$name
	fi
	md=`echo $site |base64 -i`
	b=`echo $md |wc -L`
	if [ $b -lt 36 ];then
		stin=`echo $md | md5sum |base64 -i`
	else
		stin=$md
	fi
	s=`echo $stin |cut -c1-42`
	echo ${s:0:6}-${s:12:6}-${s:18:6}-${s:24:6}-${s:30:6}-${s:36:6}
)

###########################################################################################################
echo "# This is to get the character channeling encryption scripting tool"
echo "# Autor: txgcwm"
echo -e "# \033[31m Please select a channel A=random | B=create \033[0m"

read ab

if [ "$ab" == "A" ]; then
	echo `random`
fi

if [ "$ab" == "B" ]; then
	echo -e  "\033[31m Please enter Need to encrypt a string: \033[0m"
	echo `create`
fi

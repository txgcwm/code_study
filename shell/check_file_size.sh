#! /bin/sh


check_file_size()
{
	filename=$1
	((maxsize=$2))
	
	size=`stat -c "%s" ${filename}`

	[ "${size}" -ge "${maxsize}" ] && exit 1
	
	echo "The size of ${filename} meets the requirements!"
}

check_file_size $1 0x3e8
echo "------------1------------"
check_file_size $1 400
echo "------------2------------"
check_file_size $1 256
echo "------------3------------"
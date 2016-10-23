#! /bin/sh

DIR_NAME=$1

rename_picture()
{
	cd ${DIR_NAME}

	files=`ls`

	echo ${DIR_NAME}

	for single in ${files}
	do
		filename=${single%.*}
		echo ${single} ${filename}
		mv ${single} ${filename}.jpg
	done

	cd -
}

rename_picture

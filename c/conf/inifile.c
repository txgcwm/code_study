#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "inifile.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ini_assert(_c) if(!(_c)) {printf(("ini_assert:"));assert(_c);}

#define MAX_FILE_SIZE 1024*64
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

static int load_ini_file(const char *file, char *buf, int *file_size) {
	FILE *in = NULL;
	int i = 0;
	*file_size = 0;

	 ini_assert(file != NULL);
	 ini_assert(buf != NULL);

	if (NULL == (in = fopen(file, "r"))) {
//in = fopen(file,"a"); //if the file not exist,don't create file,menghui in 2012-4-17;
//if(NULL!=in) fclose(in);
//in = fopen(file,"r");
#ifdef WIN32
		{
			char buf[256] = { 0 };
			GetCurrentDirectoryA(sizeof(buf), buf);
			PTRACERT(("Can't open file %s current directory:%s", file,
					  buf));
		}
#else
		printf(("Can't open file %s", file));
#endif

	}
	if (NULL == in) {
		return 0;
	}

	buf[i] = fgetc(in);

//load initialization file
	while (buf[i] != (char)EOF) {
		i++;
		ini_assert(i < MAX_FILE_SIZE);	//file too big, you can redefine MAX_FILE_SIZE to fit the big file
		if (i >= MAX_FILE_SIZE) {
			printf(("the file size is too big (%d)", MAX_FILE_SIZE));
			return 0;
		}
		buf[i] = fgetc(in);
	}

	*file_size = i;
	if (*file_size > MAX_FILE_SIZE) {
		buf[MAX_FILE_SIZE - 1] = '\0';
		fclose(in);
		return 0;
	}
	buf[i] = '\0';

	fclose(in);
	return 1;
}

static int newline(char c) {
	return ('\n' == c || '\r' == c) ? 1 : 0;
}

static int end_of_string(char c) {
	return '\0' == c ? 1 : 0;
}

static int left_barce(char c) {
	return LEFT_BRACE == c ? 1 : 0;
}

static int isright_brace(char c) {
	return RIGHT_BRACE == c ? 1 : 0;
}

static int parse_file(const char *section, const char *key, const char *buf,
					  int *sec_s, int *sec_e, int *key_s, int *key_e,
					  int *value_s, int *value_e) {
	const char *p = buf;
	int i = 0;
	int klen;

	ini_assert(buf != NULL);
	ini_assert(section != NULL && strlen(section));
	ini_assert(key != NULL && strlen(key));
	klen = strlen(key);
	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

	while (!end_of_string(p[i])) {
//find the section
		if ((0 == i || newline(p[i - 1])) && left_barce(p[i])) {
			int section_start = i + 1;

//find the ']'
			do {
				i++;
			} while (!isright_brace(p[i]) && !end_of_string(p[i]));

			if (0 == strncmp(p + section_start, section, i - section_start)) {
				int newline_start = 0;

				i++;

//Skip over space char after ']'
				while (isspace(p[i])) {
					i++;
				}

//find the section
				*sec_s = section_start;
				*sec_e = i;

				while (!(newline(p[i - 1]) && left_barce(p[i]))
					   && !end_of_string(p[i])) {
					int j = 0;
//get a new line
					newline_start = i;

					while (!newline(p[i]) && !end_of_string(p[i])) {
						i++;
					}

//now i is equal to end of the line
					j = newline_start;

					if (';' != p[j])	//skip over comment
					{
						while (j < i && p[j] != '=') {
							j++;
							if ('=' == p[j]) {
								if ((strncmp
									 (key, p + newline_start,
									  j - newline_start) == 0)
									&& (klen == j - newline_start)) {
//find the key ok
									*key_s = newline_start;
									*key_e = j - 1;

									*value_s = j + 1;
									*value_e = i;
									
									return 1;
								}
							}
						}
					}

					i++;
				}
			}
		} else {
			i++;
		}
	}
	
	return 0;
}

/**
*@brief read string in initialization file\n
* retrieves a string from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value
*@param value [in] pointer to the buffer that receives the retrieved string
*@param size [in] size of result's buffer
*@param default_value [in] default value of result
*@param file [in] path of the initialization file
*@return 1 : read success; \n 0 : read fail
*/
int read_profile_string(const char *section, const char *key, char *value,
						int size, const char *default_value,
						const char *file) {
	char *buf = NULL;
	int file_size;
	int sec_s, sec_e, key_s, key_e, value_s, value_e;

//check parameters
	ini_assert(section != NULL && strlen(section));
	ini_assert(key != NULL && strlen(key));
	ini_assert(value != NULL);
	ini_assert(size > 0);
	ini_assert(file != NULL && strlen(key));

	buf = (char *)calloc(MAX_FILE_SIZE, 1);
	if (buf == NULL) {
		return 0;
	}

	if (!load_ini_file(file, buf, &file_size)) {
		if (default_value != NULL) {
			strncpy(value, default_value, size);
		}
		free(buf);
		return 0;
	}

	if (!parse_file
		(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s,
		 &value_e)) {
		if (default_value != NULL) {
			strncpy(value, default_value, size);
		}
		free(buf);
		return 0;			//not find the key
	} else {
		int cpcount = value_e - value_s;

		if (size - 1 < cpcount) {
			cpcount = size - 1;
		}

		memset(value, 0, size);
		memcpy(value, buf + value_s, cpcount);
		value[cpcount] = '\0';
		free(buf);
		return 1;
	}
	free(buf);
	return 0;
}

/**
*@brief read int value in initialization file\n
* retrieves int value from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value
*@param default_value [in] default value of result
*@param file [in] path of the initialization file
*@return profile int value,if read fail, return default value
*/
int read_profile_int(const char *section, const char *key,
					 int default_value, const char *file) {
	char value[32] = { 0 };
	if (!read_profile_string
		(section, key, value, sizeof(value), NULL, file)) {
		return default_value;
	} else {
		return atoi(value);
	}
}

/**
* @brief write a profile string to a ini file
* @param section [in] name of the section,can't be NULL and empty string
* @param key [in] name of the key pairs to value, can't be NULL and empty string
* @param value [in] profile string value
* @param file [in] path of ini file
* @return 1 : success\n 0 : failure
*/
int write_profile_string(const char *section, const char *key,
						 const char *value, const char *file) {
	char *buf = NULL;		//[MAX_FILE_SIZE]={0};
	char *w_buf = NULL;		//[MAX_FILE_SIZE]={0};
	int sec_s, sec_e, key_s, key_e, value_s, value_e;
	int value_len = (int)strlen(value);
	int file_size = 0;
	FILE *out;

//check parameters
	ini_assert(section != NULL && strlen(section));
	ini_assert(key != NULL && strlen(key));
	ini_assert(value != NULL);
	ini_assert(file != NULL && strlen(key));

	buf = (char *)calloc(MAX_FILE_SIZE, 1);
	if (buf == NULL) {
		
		return 0;
	}
	w_buf = (char *)calloc(MAX_FILE_SIZE, 1);
	if (w_buf == NULL) {
		free(buf);
		
		return 0;
	}

	if (!load_ini_file(file, buf, &file_size)) {
		sec_s = -1;
	} else {
		parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e,
				   &value_s, &value_e);
	}

	if (-1 == sec_s) {
		if (0 == file_size) {
			sprintf(w_buf + file_size, "[%s]\n%s=%s\n", section, key,
					value);
		} else {
//not find the section, then add the new section at end of the file
			memcpy(w_buf, buf, file_size);
			sprintf(w_buf + file_size, "\n[%s]\n%s=%s\n", section, key,
					value);
		}
	} else if (-1 == key_s) {
//not find the key, then add the new key=value at end of the section
		memcpy(w_buf, buf, sec_e);
		sprintf(w_buf + sec_e, "%s=%s\n", key, value);
		sprintf(w_buf + sec_e + strlen(key) + strlen(value) + 2,
				buf + sec_e, file_size - sec_e);
	} else {
//update value with new value
		memcpy(w_buf, buf, value_s);
		memcpy(w_buf + value_s, value, value_len);
		memcpy(w_buf + value_s + value_len, buf + value_e,
			   file_size - value_e);
	}

	out = fopen(file, "w");
	if (NULL == out) {
		free(buf);
		free(w_buf);
		
		return 0;
	}

	if (-1 == fputs(w_buf, out)) {
		fclose(out);
		free(buf);
		free(w_buf);
		
		return 0;
	}

	free(buf);
	free(w_buf);
	fclose(out);
	
	return 1;
}

#ifdef __cplusplus
};								//end of extern "C" {
#endif

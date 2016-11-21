#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1024

enum FLAG_HEAD {
	VERSION,
	COEX_SCHEME,
	PATCH,
};

int main(int argc, char ** argv)
{
	enum FLAG_HEAD flag = PATCH;
	int i = 0, j = 0;
	int offset = 0;
	int end = 0;
	//printf("argc: %d\r\n", argc);
	if (argc != 2) {
		printf("format: ./pskey_format inputfile\r\n");
		return 0;
	}
	const char comment_head_size = strlen("/*");
	const char comment_end_size = strlen("*/\r\n");
	const char data_head_size = strlen("{ 1,(Byte_t *)\"");
	const char data_end_size = strlen("\"},\r\n");

	char	buf[MAXLINE] = {0};
	char* comment_str[100] = {0};

	FILE *fp = NULL;
	FILE *fp_new = NULL;
	if ((fp = fopen(argv[1], "r")) != NULL) {
		if ( (fp_new = fopen("PS_KEY_CSR8811_gen.txt", "w") ) == NULL) {
			goto error_fp;
		}

	} else { goto error_inputfile; }

	while (fgets(buf, MAXLINE, fp) != NULL) {
		if (fp == NULL) {
			puts("Couldn't open file");
			exit(0);
		} else {
			i=0;
			j=0;
			offset = 0;
			end = strlen(buf);
			while(buf[i] == ' ' || buf[i] == '\t'){
				i++;
			}
			offset += i;
			if(buf[i] == '/' && buf[i+1] == '*') {
				if (strstr(buf, "VERSION") != NULL) {
					flag = VERSION;
				} else if (strstr(buf, "COEX_SCHEME") != NULL) {
					flag = COEX_SCHEME;
				} else if (strstr(buf, "PATCH") != NULL) {
					flag = PATCH;
				} else {
					flag = PATCH;
					//goto error_file_format;
				}

				offset = i+1;
				buf[offset] = '#';

				//printf("buf[%d]:%x\r\n", end -2, buf[end-2]);
				//printf("buf[%d]:%x \r\n", end -1, buf[end-1]);
				if(buf[end-comment_end_size+1 ] == '/' && buf[end-comment_end_size] == '*') {
					j = end-comment_end_size-1;
					while(buf[j] == ' ') {
						j--;
					}
					buf[j+1] = '\r';
					buf[j+2] = '\n';
					buf[j+3] = '\0';
				}
				fputs(buf+offset, fp_new);
			} else if (buf[i] == '{') {
				offset +=  (data_head_size - 4);
				if (flag == VERSION || flag == COEX_SCHEME) {

					memcpy(buf+offset,"\\x06",4);
				} else if (flag == PATCH) {
					memcpy(buf+offset,"\\x05",4);
				} else {
					goto error_file_format;
				}
				buf[end-data_end_size] = '\r';
				buf[end-data_end_size+1] = '\n';
				buf[end-data_end_size+2] = '\0';
				fputs(buf+offset, fp_new);
			} else {
				printf("error format!\r\n");

			}
		}
	}

	fputs("# WARM RESET\r\n", fp_new);
	fputs("\\x00\\xC2\\x02\\x00\\x09\\x00\\x47\\x00\\x02\\x40\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\r\n", fp_new);
	puts("Done");
	fclose(fp_new);
error_fp:
	fclose(fp);
	return 0;
error_file_format:
	printf("error format!\r\n");
error_inputfile:
	printf("error input file!\r\n");
	return 0;
}

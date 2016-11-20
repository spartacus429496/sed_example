#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1024
enum FLAG_HEAD {
	VERSION,
	COEX_SCHEME,
	PATCH,
};
char* commet_str[100] = {0};
int main(int argc, char ** argv)
{
	enum FLAG_HEAD flag = PATCH;
	int i = 0, j = 0;
	int offset = 0;
	int end = 0;
	printf("argc: %d\r\n", argc);
	if (argc < 2) {
		printf("format: ./exe filename\r\n");
		return 0;
	}
	for(i=0;i<argc;i++) {
		printf("argv[%d]:%s\r\n", i, argv[i]);

	}
	char	buf[MAXLINE];
	//FILE *fp = fopen("test.txt", "w");
	//FILE *fp = fopen("origin.txt", "r");
	FILE *fp = fopen(argv[1], "r");
	//FILE *fp_new = fopen("origin_new.txt", "a+");
	FILE *fp_new = fopen("origin_new.txt", "w");
	//char	buf[MAXLINE];

	while (fgets(buf, MAXLINE, fp) != NULL)
	{
		if (fp == NULL)
		{
			puts("Couldn't open file");
			exit(0);
		}
		else
		{
			i=0;
			end = strlen(buf);
			while(buf[i] == ' '){
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
					goto error_file_format;
					//printf("error format!\r\n");
				}

				offset = i+1;
				buf[offset] = '#';

		//	if (fscanf(buf+offset, "%*s %*s %s ", commet_str)==1) {
		//		printf("commets:%s\r\n", commet_str);

		//	}
			printf("buf[%d]:%x\r\n", end -2, buf[end-2]);
			printf("buf[%d]:%x \r\n", end -1, buf[end-1]);
			if(buf[end-2-1] == '/' && buf[end-2-1-1] == '*') {
				j = end-2-1-1-1;
				while(buf[j] == ' ') {
					j--;

				}
				buf[j+1] = '\r';
				buf[j+1+1] = '\n';
				buf[j+1+2] = '\0';
			}
				fputs(buf+offset, fp_new);
			} else if (buf[i] == '{') {
				offset +=  strlen("{ 1,(Byte_t *)\"");
				if (flag == VERSION || flag == COEX_SCHEME) {

					memcpy(buf+offset-4,"\\x06",4);
				} else if (flag == PATCH) {
					memcpy(buf+offset-4,"\\x05",4);
				} else {
					goto error_file_format;
				}
				//buf[end-1-2] = 0;
				buf[end-2-3] = '\r';
				buf[end-2-3+1] = '\n';
				buf[end-2-3+2] = '\0';
				fputs(buf+offset-4, fp_new);
			} else {
				printf("error format!\r\n");

			}
			//fputs("GeeksforGeeks", fp_new);
			//fputs(buf,fp_new);
			//puts("Done");
			//fclose(fp);
		}
	}

	fputs("# WARM RESET\r\n", fp_new);
	fputs("\\x00\\xC2\\x02\\x00\\x09\\x00\\x47\\x00\\x02\\x40\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\r\n", fp_new);
	puts("Done");
	fclose(fp);
	fclose(fp_new);
	return 0;
error_file_format:
	printf("error format!\r\n");
error_param:
	return 0;
}

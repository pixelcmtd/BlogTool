#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>

using namespace std;

long fsize(char *file)
{
	struct stat s;
	int rc = stat(file, &s);
	return rc == 0 ? s.st_size : -1;
}

int main(int argc, char **argv)
{
	if(argc < 5)
	{
		printf("%s [title] [raw text file] ", argv[0]);
		puts("[formatted text file] [html file]\n");
		return 1;
	}
	long rtxtl = fsize(argv[2]);
	char *rtxt = (char*)malloc(rtxtl);
	FILE *f = fopen(argv[2], "r");
	rtxtl = fread(rtxt, 1, rtxtl, f);
	fclose(f);
	f = fopen(argv[3], "w");
	FILE *g = fopen(argv[4], "w");
	fputs(argv[1], f);
	fputc('\n', f);
	for(unsigned i = 0; i < strlen(argv[1]); i++)
		fputc('-', f);
	fputc('\n', f);
	fputs("<head><title>", g);
	fputs(argv[1], g);
	fputs("</title></head><body><h1>", g);
	fputs(argv[1], g);
	fputs("</h1><p>", g);
	bool urllabel = false;
	bool urlurl = false;
	vector<char> str;
	for(long i = 0; i < rtxtl; i++)
	{
		char c = rtxt[i];
		if(urllabel && c == ']')
		{
			urllabel = 0;
		}
		else if(urlurl && c == '}')
		{
			urlurl = 0;
			fputs("\">", g);
			fputs(&str[0], g);
			fputs("</a>", g);
			fputc(')', f);
		}
		else if(urllabel)
		{
			str.push_back(c);
			fputc(c, f);
		}
		else if(urlurl)
		{
			fputc(c, f);
			fputc(c, g);
		}
		else if(c == '[')
		{
			urllabel = 1;
		}
		else if(c == '{')
		{
			urlurl = 1;
			fputc('(', f);
			fputs("<a href=\"", g);
		}
		else if(c == '\n')
		{
			fputs("</p><p>", g);
			fputc(c, f);
		}
		else if(c != '\r')
		{
			fputc(c, f);
			fputc(c, g);
		}
	}
	fclose(f);
	fputs("</p></body>", g);
	fclose(g);
	return 0;
}

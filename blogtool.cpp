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
	char *title = argv[1];
	char *rtxtf = argv[2];
	char *ftxtf = argv[3];
	char *htmlf = argv[4];
	long rwtxtlen = fsize(rtxtf);
	char *rwtxt = (char*)malloc(rwtxtlen);
	FILE *f = fopen(rtxtf, "r");
	rwtxtlen = fread(rwtxt, 1, rwtxtlen, f);
	fclose(f);
	f = fopen(ftxtf, "w");
	FILE *g = fopen(htmlf, "w");
	fputs(title, f);
	fputc('\n', f);
	for(int i = 0; i < strlen(title); i++)
		fputc('-', f);
	fputc('\n', f);
	fputs("<head><title>", g);
	fputs(title, g);
	fputs("</title></head><body><h1>", g);
	fputs(title, g);
	fputs("</h1><p>", g);
	bool urllabel = false;
	bool urlurl = false;
	vector<char> str;
	for(long i = 0; i < rwtxtlen; i++)
	{
		char c = rwtxt[i];
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
		else if(c == '\r');
		else if(c == '\n')
		{
			fputs("</p><p>", g);
			fputc(c, f);
		}
		else
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

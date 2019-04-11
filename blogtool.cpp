#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char **argv)
{
	if(argc < 5)
	{
		printf("%s [title] [raw text file] ", argv[0]);
		puts("[formatted text file] [html file]\n");
		return 1;
	}
	FILE *h = fopen(argv[2], "r");
	FILE *f = fopen(argv[3], "w");
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
	fputs("</h1>", g);
	bool urllabel = false;
	bool urlurl = false;
	vector<char> str;
	int i = 0;
	while((i = fgetc(h)) != -1)
	{
		if(urllabel && i == ']')
			urllabel = 0;
		else if(urlurl && i == '}')
		{
			urlurl = 0;
			fputs("\">", g);
			fputs(&str[0], g);
			fputs("</a>", g);
			fputc(')', f);
		}
		else if(urllabel)
		{
			str.push_back(i);
			fputc(i, f);
		}
		else if(urlurl)
		{
			fputc(i, f);
			fputc(i, g);
		}
		else if(i == '[')
			urllabel = 1;
		else if(i == '{')
		{
			urlurl = 1;
			fputc('(', f);
			fputs("<a href=\"", g);
		}
		else if(i == '\n')
		{
			fputs("<br/>", g);
			fputc(i, f);
		}
		else if(i != '\r')
		{
			fputc(i, f);
			fputc(i, g);
		}
	}
	fclose(f);
	fputs("</body>", g);
	fclose(g);
	fclose(h);
	return 0;
}

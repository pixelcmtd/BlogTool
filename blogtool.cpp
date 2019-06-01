#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

int help(char **argv)
{
	printf("%s create\n", argv[0]);
	printf("%s new [title] [raw text file] ", argv[0]);
	puts("[formatted text file] [html file]");
	return 1;
}

int err(int enset)
{
	if(enset) printf("errno: %d\n", errno);
	else puts("ERROR, ABORTING");
	return 2;
}

char *dir_name()
{
	char *dn = (char *)malloc(PATH_MAX + 5);
	getcwd(dn, PATH_MAX);
	char *c = dn + strlen(dn);
	*c++ = '/';
	*c++ = '.';
	*c++ = 'c';
	*c++ = 'b';
	*c   = 't';
	return dn;
}

int main(int argc, char **argv)
{
	if(argc < 2) return help(argv);
	if(*argv[1] == 'n')
	{
		if(argc < 6) return help(argv);
		argv++;
		FILE *h = fopen(argv[2], "r");
		if(!h) return err(1);
		FILE *f = fopen(argv[3], "w");
		if(!f) return err(1);
		FILE *g = fopen(argv[4], "w");
		if(!g) return err(1);
		if(fputs(argv[1], f) == EOF) return err(0);
		fputc('\n', f);
		for(unsigned i = 0; i < strlen(argv[1]); i++)
			fputc('-', f);
		fputc('\n', f);
		fputs("<html><head><title>", g);
		fputs(argv[1], g);
		fputs("</title></head><body><h1>", g);
		fputs(argv[1], g);
		fputs("</h1>", g);
		bool urllabel = false;
		bool urlurl = false;
		bool lastnewline = false;
		vector<char> str;
		int i = 0;
		while((i = fgetc(h)) != -1)
		{
			if(urllabel && i == ']')
				str.push_back('\0'), urllabel = 0;
			else if(urlurl && i == ')')
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
			else if(i == '(')
			{
				urlurl = 1;
				fputc('(', f);
				fputs("<a href=\"", g);
			}
			else if(i == '\n')
			{
				if(lastnewline) fputs("<br/>", g);
				else fputc(' ', g);
				fputc(i, f);
			}
			else if(i != '\r')
			{
				fputc(i, f);
				fputc(i, g);
			}
			if(i == '\n') lastnewline = true;
			else lastnewline = false;
		}
		fclose(f);
		fputs("</body></html>", g);
		fclose(g);
		fclose(h);
		puts("wrote article.");
		return 0;
	}
	else if(*argv[1] == 'c')
	{
		char *dn = dir_name();
		mkdir(dn, 755);
		free(dn);
	}
	else help(argv);
}

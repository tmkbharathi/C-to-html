#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
void preprocessor(char *buffer, int size, FILE *fd);
int keywordcheck(char*buffer);
char operatorcheck(char*buffer);

char keyword[32][10]={"auto","break","case","char","const","continue","default",
                                                "do","double","else","enum","extern","float","for","goto",
                                                "if","int","long","register","return","short","signed",
                                                "sizeof","static","struct","switch","typedef","union",
                                                "unsigned","void","volatile","while"};
char formatspecifier[8][3] = {"%d", "%f", "%s", "%c", "%ld", "%x", "%o", "%i"};
char operator[16] = "+-*/(){}<>&=[],;";


int main(int argc, char *argv[])
{
	int i = 0, ch, dist, flag;
	FILE *fp1, *fp, *fd;

	char *buffer;
	char word[25];

	if (argc == 1)
    {
		printf("Usage: %s <.c file>\n", argv[0]);
		return 0;
	}

  printf("----Lexical Analyser----\n");
	fp = fopen(argv[1], "r");
	fp1 = fopen(argv[1], "r");
	fd = fopen("new.html", "w");

	fprintf(fd, "<!DOCTYPE html>\n");
	fprintf(fd, "<html lang=\"en-US\">\n");
	fprintf(fd, "<head>\n");
	fprintf(fd, "<title>%s</title>\n", "ccode2htmlcode");
	fprintf(fd, "<meta charset=\"UTF-8\">\n");
	fprintf(fd, "</head>\n");
	fprintf(fd, "<body style=\"background-color:black;\">\n");
	fprintf(fd, "<pre>\n");

	while((ch = fgetc(fp1)) != EOF)
	{
		if (ch == '\n')
		{
			dist = ftell(fp1) - ftell(fp) - 1;

			buffer = malloc((dist) * sizeof(char));
			fseek(fp1, -(dist + 1), SEEK_CUR);
			fread(buffer, dist, sizeof(char), fp1);
			if(buffer[0] == '#')
			{
				fprintf(fd,"<span style='color: purple'>%s</span>",buffer);
				preprocessor(buffer, dist, fd);
				fprintf(fd, "\n");
			}
			else
			{
				fprintf(fd, "\n");
				i = 0;
				while(i < dist)
				{
					char ch = '\0';
					int k = 0;
					if(buffer[i] == 34)
					{
						printf("Literal : ");
						do
						{
							fprintf(fd,"<span style='color: green'>%c</span>", buffer[i]);
							printf("%c", buffer[i]);
							i++;
						}while(buffer[i] != 34);
						printf("\"\n");
						fprintf(fd,"<span style='color: green'>%c</span>", buffer[i]);
					}
					if(buffer[i] == '/' && buffer[i + 1] == '/')
					{
						while(i < dist)
						{
							fprintf(fd,"<span style='color: blue'>%c</span>", buffer[i]);
							i++;
						}
						continue;
					}
					if(buffer[i] == 39)
					{
						printf("character constant: ");
						do
						{
							printf("%c", buffer[i]);
							fprintf(fd,"<span style='color: firebrick'>%c</span>", buffer[i]);
							i++;
						}while(buffer[i] != 39);
						fprintf(fd,"<span style='color: firebrick'>%c</span>", buffer[i]);
					}
					if(buffer[i] == ' ')
					{
						fprintf(fd," ");
					}
					if(buffer[i] == '\t')
					{
						fprintf(fd,"\t");
					}
					while (buffer[i] != ' ' && buffer[i] != '\t' && !(ch = operatorcheck(&buffer[i])))
					{
						word[k++] = buffer[i++];
						if(i == dist)
						{
							break;
						}
					}
					word[k] = '\0';
					if (isdigit(word[0]))
					{

						fprintf(fd,"<span style='color: darkgreen'>%s</span>", word);
						printf("constant : %s \n", word);
					}
					else if (isalpha(word[0]))
					{
						flag = keywordcheck(word);
						if(ch == '\0')
						{
							if(flag)
							{
								fprintf(fd,"<span style='color: white'>%s </span>", word);
							}
							else
							{
								fprintf(fd,"<span style='color: GoldenRod'>%s </span>", word);
							}
						}
						else
						{
							if(flag)
							{
								fprintf(fd,"<span style='color: white'>%s</span>", word);
							}
							else
							{
								fprintf(fd,"<span style='color: GoldenRod'>%s</span>", word);
							}
						}
					}
					if (ch != '\0')
					{
						printf("operator : %c \n",ch);
						fprintf(fd,"<span style='color: Fuchsia'>%c</span>", ch);
						ch = '\0';
					}
					i++;
					for(k = 0; k <25; k++)
					{
						word[k] = '\0';
					}
				}
      }
			free(buffer);
			fseek(fp1,1,SEEK_CUR);
			fseek(fp, dist + 1, SEEK_CUR);
		}
	}
	fprintf(fd, "</pre>\n");
	fprintf(fd, "</body>\n");
	fprintf(fd, "</html>\n");
  fclose(fd);
  fclose(fp1);
  fclose(fp);
}

void preprocessor(char *buffer, int size, FILE *fd)
{
	int i = 0, j;
	if(buffer[i] == '#')
	{
		printf("Preprocessor directive : ");
		while(buffer[i] != ' ')
		{
			printf("%c", buffer[i]);
			i++;
		}
		printf("\n");
		i++;
		if(buffer[i] == '<' || buffer[i] == '"')
		{
			printf("Header file : %s\n", &buffer[i]);
			j = i;
			while(buffer[j] != '\0')
			{
				fprintf(fd,"<span style='color: red'>%c</span>", buffer[j]);
				j++;
			}
		}
	}
}

int keywordcheck(char *buffer)
{
	for (int i = 0; i < 32; i++)
	{
		char *key = (char *)malloc(strlen(keyword[i]));;
		strcpy(key, keyword[i]);
		if (strcmp(buffer,key) == 0)
		{
			printf("Keyword : %s\n", buffer);
			free(key);
			return 0;
		}
		free(key);
	}
	printf("Identifier: %s\n", buffer);
	return -1;
}


char operatorcheck(char*buffer)
{
	for (int i = 0; i < 16; i++)
	{
		if (*buffer == operator[i])
		{
			return *buffer;
		}
	}
	return 0;
}

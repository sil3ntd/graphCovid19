/* Requirement: 

   	Create a vertical histogram of the daily covid19 cases for each country. Use
	the dataset from the Johns Hopkins University.


	One approach to solve the problem is to divide it into three separate
	functions. They are:

	 		We get the raw data for each country
			Format the data
			create the graph
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>

#define MAXLEN 2300		/* maximum length of each record */
#define MAXFIELD 300	/* maximum number of fields */
#define MAXRECORD 300	/* maximum number of records */
#define MAXLENGTH 50	/* maximum length of a country name */


int get_record(char *);
int get_fields(char *, char *[]);
void print_usage(char *);
void draw_graph(int [], int );
void print_list(char [][MAXLENGTH], int size);

FILE *fp;

int main(int argc, char *argv[])
{
	char record[MAXLEN], header[MAXFIELD][MAXLENGTH], *fields[MAXFIELD];
	char country[MAXRECORD][MAXLENGTH];		/* array of country names */
	char c;
	int h_opt = 0, l_opt = 0, c_opt = 0;
	char *prog_name = argv[0];
	char buff[50];
	int i,len, count = 0;
	int cases[1000] = {0};
	int found = 0;
	char start_date[20], end_date[20], highest_day[20];
	int highest_case = 0;

	setlocale(LC_NUMERIC, "");

	fp = fopen("data.csv", "r");
	if(!fp){
		printf("data.csv does not exist\n");
		return 0;
	}

	while(--argc > 0 && *(++argv)[0] == '-'){
		c = *++argv[0];
		switch(c){
			case 'h':
				h_opt = 1;
				break;
			case 'l':
				l_opt = 1;
				break;
			case 'c':
				c_opt = 1;
				break;
			default:
				printf("%s: Invalid option -- '%c'\n", prog_name, c);
				printf("Try '%s -h' for more information\n", prog_name);
				return -1;
		}
	}
	if(argc == 0){
		if(l_opt){
			printf("\nList of countries:\n\n");
			/* 
			   Structure for printing the list of countries:
		
			   get a record
			   separate into fields
			   save all the country fields
			   sort them
			   print a unique list
			*/
			count = 0;
			while(get_record(record) > 0){
				get_fields(record, fields);
				strcpy(country[count++], fields[1]);
			}
			print_list(country, count-1);
		}else if(h_opt)
			print_usage(prog_name);
		else if(c_opt){
			printf("%s: Too few arguments\n", prog_name);
			printf("Try '%s -h' for more information\n", prog_name);
			return -1;
		}else
			print_usage(prog_name);
	}else{
		if(c_opt){
			/* Save the header */
			get_record(record);
			len = get_fields(record, fields);
			for(i = 0; i < len; i++)
				strcpy(header[i], fields[i]);

			/* Save the start and end dates */
			strcpy(start_date, header[4]);
			strcpy(end_date, header[len - 1]);

			/* get the country */
			strcpy(buff, argv[0]);

			/* 
			   Structure for printing the graph for a country:

			   get a record
			   separate into fields
			   save all the data from specified country
			   print the graph using the data
			*/
			while(get_record(record) > 0){
				len = get_fields(record, fields);
				if(strcmp(fields[1], buff) == 0){
					found = 1;
					/* start collecting from the 4th element  and record the
					 * highest case */
					cases[0] += atoi(fields[4]);
					highest_case = atoi(fields[4]);
					for(i = 1; i < len - 4; i++){
						cases[i] += (atoi(fields[i+4]) - atoi(fields[i+4-1]));
						if(cases[i] > highest_case){
							highest_case = cases[i];
							strcpy(highest_day, header[i+4]);
						}
					}
				}
			}
		
			if(!found){
				printf("%s: country '%s' not found\n", prog_name, argv[0]);
				printf("Try '%s -l' to see the list of countries\n", prog_name);
				return -1;
			}
			printf("\n");
			draw_graph(cases, len - 4);

			printf("\n\tGraph of covid-19 daily cases\n");
			printf("\tDate: %s20 to %s20\n", start_date, end_date);
			printf("\tCountry: %s\n\n", buff);
			printf("\tLast recorded case: %'d cases\n", cases[i-1]);
			printf("\tHighest recorded case in a day was at %s20 wth %'d cases\n\n", highest_day, highest_case);

		}else{
			printf("%s: Too many arguments\n", prog_name);
			printf("Try '%s -h' for more information\n", prog_name);
			return -1;
		}
	}
	fclose(fp);
	return 0;
}

void print_usage(char *s)
{
	printf("Usage: %s [OPTIONS]\n", s);
	printf("Display a histogram of covid-19 cases for each country\n");
	printf("\nOptions:\n");
	printf("%3s %-12s Print the graph for a specific country.\n", "-c",
			"COUNTRY");
	printf("%3s %12c Prints a list of countries.\n", "-l", 0x20);
	printf("%3s %12c Prints this usage information.\n", "-h", 0x20);

	printf("\n");
}



int cmp(const void *a, const void *b)
{
	return strcmp((char*)a, (char*)b);
}
/* print_list: print a unique list of country name from an array of country
 * names */

void print_list(char s[][MAXLENGTH], int size)
{
	int i, count;
	char buff[MAXLENGTH];

	qsort((void*)s, size, sizeof(s[0]), cmp);

	for(i = 0, count = 0; i < size; i++){
		if(strcmp(buff, s[i]) == 0)
			;
		else{
			printf("%-33s", s[i]);
			strcpy(buff, s[i]);
			count++;
			if((count % 5) == 0){
				printf("\n");
				count = 0;
			}
		}
	}
	printf("\n");
}

/* get_record: get a record from the csv data, save into s and return the total
 * number of characters read */
int get_record(char *s)
{
	int i, c;

	i = 0;
	while((c = fgetc(fp)) != EOF && c != '\n')
		s[i++] = c;

	if(c == '\n')
		s[i++] = c;

	s[i] = '\0';
	return i;
}

/* get_fields: separate s into fields and save the pointer pointing at the start
 * of each field into t. Note that this function modifies the content of s */

int get_fields(char *s, char *t[])
{
	char *p_cur; 		/* current pointer in s */
	char *p_start;		/* pointer at the start of a field */
	char delim = 0x2C; 	/* initialize delimeter to the comma (,) character */
	int j;

	p_start = s;		
	/* Check if the current character is a quote ("), if it is start reading at
	 * the next character, and set delimeter as the quote (") character */
	if(*p_start == 0x22){	 
		delim = 0x22;		
		++p_start;
	}

	for(p_cur = p_start, j = 0; *p_cur != '\0'; ++p_cur){
		if(*p_cur == delim){
			t[j++] = p_start;
			*p_cur = '\0';
			if(delim == 0x22)
				++p_cur;
			p_start = p_cur + 1;
			/* Check if the current character  is a quote ("), if true start
			 * reading at the next character, and set the delimeter as the
			 * quote (") character otherwise set comma (,) as the delimiter */
			if(*p_start == 0x22){
				delim = 0x22;
				++p_cur;
				p_start = p_cur + 1;
			}else
				delim = 0x2C;
		}
	}
	/* At this point the last field is not saved since there in no delimeter at
	 * the end of that field. So we save the field manually. Currently p_cur
	 * points to the null character. Make the p_cur point to the character
	 * before the null character which is a newline character and change the
	 * newline character to a null character to complete the string */
	*--p_cur = '\0';
	t[j++] = p_start;
	t[j] = NULL;
	return j;
}

void draw_graph(int data[], int size)
{
	int i, base = 0, largest = 0;

	/* find the largest element */
	for(i = 0; i < size; i++)
		largest = largest > data[i] ? largest : data[i];
	base = largest/15;
	while(base > 0){
		printf("%4c|", 0x20);
		for(i = 0; i < size; i++){
			if(data[i]/15 < base)
				printf("%c", 0x20);
			else
				printf("%c", '|');
		}
		--base;
		printf("\n");
	}
	/* Put a header line on the x axis */
	printf("%4c|", 0x20);	
	for(i = 0; i < size; i++)
		printf("_");
	printf("____\n");
	
}

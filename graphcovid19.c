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
#define MAXLEN 1000


int getraw(char *);
int unraw(char *, char *[]);
void draw_graph(int data[], int size);

main(int argc, char *argv[])
{
	int i, j;
	char raw[MAXLEN];
	char *unraw_data[100];
	int len = 0;
	int case_per_day[100] = {0};

	/* Get the header data */
	getraw(raw);
	len = unraw(raw, unraw_data);
	
	printf("\n\n%4c Data is from %s - %s", 0x20, unraw_data[4], unraw_data[len - 2]);


	while((len = getraw(raw)) > 0){
		/* Select only the data from a specific country specified in the command
		 * line argument */
		if(strstr(raw, argv[1])){
			unraw(raw, unraw_data);
			/* save the number of cases per day, starting from the jth element
			 * of unraw_data array */
			i = j = strcmp(argv[1], "Korea, South") ? 4 : 5;
			case_per_day[i-j] += atoi(unraw_data[i]);
			++i;
			while(unraw_data[i] != NULL){
				case_per_day[i-j] += (atoi(unraw_data[i]) - atoi(unraw_data[i-1]));	
				i++;
			}
		}
	}
	printf("%4c Graph of cases per day.\n%4c Country: %s\n", 0x20,
			0x20, argv[1]);

	draw_graph(case_per_day, i - j);

	printf("\n");
	return 0;
}

/* get raw/csv data and write into s, return total number of
 * characters read */
int getraw(char *s)
{
	int i, c;

	i = 0;
	while((c = getchar()) != EOF && c != '\n')
		s[i++] = c;

	if(c == '\n')
		s[i++] = c;

	s[i] = '\0';
	return i;
}

/* unraw: get each item from the csv formatted raw data and save it
 * to the unraw_data. */
int unraw(char *raw, char *unraw_data[])
{
	int i = 0;
	char *t;
	
	if(raw[i] == ',')
		unraw_data[i++] = ",";

	t = strtok(raw, ",");
	unraw_data[i++] = t;
	while(t != NULL){
		t = strtok(NULL, ",");
		unraw_data[i++] = t;
	}
	unraw_data[i] = t;
	return i;
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
	printf("%c|", 0x20);	
	for(i = 0; i < size; i++)
		printf("_");
	printf("____\n");
	
}

#include <stdio.h>
#include <string.h>

/*int getraw(char *);*/

void draw_graph(int data[], int size);

main(int argc, char *argv[])
{
	int c, i, j;
	char raw[1000];
	char *unraw_data[100];
	int len = 0;
	int case_per_day[100];

	while((len = getraw(raw)) > 0){
		if(strstr(raw, argv[1])){
			unraw(raw, unraw_data);

			/* save the number of cases per day */
			i = 4;
			while(unraw_data[i] != NULL){
				if(i == 4)
					case_per_day[i-3] += atoi(unraw_data[i]);
				else
					case_per_day[i-3] += (atoi(unraw_data[i]) -
						atoi(unraw_data[i-1]));	
				i++;
			}
		}
	}
	draw_graph(case_per_day, i - 3);


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

	s[i] = '\0';
	return i;
}

/* unraw: get each item from the csv formatted raw data and save it
 * to the unraw_data. */
int unraw(char *raw, char *unraw_data[])
{
	int i = 0;
	char *t;
	
	if(raw[0] == ',')
		unraw_data[i++] = ",";

	t = strtok(raw, ",");
	unraw_data[i++] = t;
	while(t != NULL){
		t = strtok(NULL, ",");
		unraw_data[i++] = t;
	}
	/*		
	for(i = 0; unraw_data[i] != '\0'; ++i)
		printf("data[%d] - %s\n", i, unraw_data[i]);
	*/
	

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
				printf("%2c", 0x20);
			else
				printf("%2c", '|');
		}
		--base;
		printf("\n");
	}
	/* Put a header on the x axis */
	printf("%4c|", 0x20);	
	for(i = 0; i < size; i++)
		printf("__");
	
	printf("____\n");
	
}

This is a very simple program that will create a graph of the cases
per day per country. The graph can be viewed on your terminal. 

This is an alternative way of viewing the graph aside from the
web-based graph which can be seen from the Johns Hopkins website.

I created this because I have a slow internet connection and loading
from the Johns Hopkins website takes some time. With this you can
just download the data which is in text format (csv file) and run the program.

## Screenshot
![Preview](./screenshot1.png)

To compile the program:

	$ c89 graphcovid19.c -o graphCovid19

To view help:

	$ ./graphcovid19 -h

To get a list of countries:

	$ ./graphcovid19 -l

To view the graph for a specific country, e.g. Philippines

	$ ./graphcovid19 -c Philippines


To download the data:

	$ curl https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/time_series_covid19_confirmed_global.csv > data.csv

It is best to download first the data before running the program to
have the latest information. 

The data is taken from  https://github.com/CSSEGISandData/COVID-19/

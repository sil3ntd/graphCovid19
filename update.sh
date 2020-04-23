#!/bin/sh

mv data.csv data`date "+%Y%m%d-%H%M%S"`.csv

curl \
	https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/time_series_covid19_confirmed_global.csv \
	> data.csv

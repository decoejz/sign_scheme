from raw import process_raw_data
from split_data import split_data
from graph import graph_generator, boxplot_generator
from matrix import correlation_analysis

csv_file_path = "data/teste.csv"
raw_data = process_raw_data(csv_file_path)
splitted_data = split_data(raw_data)

graph_generator(splitted_data)
boxplot_generator(splitted_data)
correlation_analysis(splitted_data)

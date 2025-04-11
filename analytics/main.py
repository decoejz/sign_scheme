from raw import process_raw_data
from split_data import split_data
from graph import graph_generator, boxplot_generator, normalized_graph_generator
from matrix import correlation_analysis

# csv_file_path = "data/teste.csv"
# csv_file_path = "data/qgc_s1_v1.csv"
# csv_file_path = "data/px4_s1_v1.csv"
csv_file_path = "data/s1_v1.csv"

raw_data = process_raw_data(csv_file_path)
splitted_data = split_data(raw_data)

graph_generator(splitted_data)
# normalized_graph_generator(splitted_data)
boxplot_generator(splitted_data)
# correlation_analysis(splitted_data)

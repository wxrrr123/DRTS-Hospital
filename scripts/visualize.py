import re
import os
import sys
import glob
import matplotlib.pyplot as plt
from datetime import datetime

pattern = r"simulation_(\d{4}-\d{2}-\d{2}-\d{6})\.txt"
directory = "output"
search_path = os.path.join(directory, "simulation_*.txt")

files = glob.glob(search_path)
file_times = []

for file in files:
    print(file)
    match = re.search(pattern, os.path.basename(file))
    if match:
        timestamp_str = match.group(1)
        timestamp = datetime.strptime(timestamp_str, "%Y-%m-%d-%H%M%S")
        file_times.append((file, timestamp))

if file_times:
    max_file = max(file_times, key=lambda x: x[1])
    filename = max(file_times, key=lambda x: x[1])[0]
    timestamp = max(file_times, key=lambda x: x[1])[1]
else:
    print("Cannot find any output file", file=sys.stderr)
    sys.exit(1)

generations = []
average_fitness = []
best_fitness = []
highest_generation = -1
highest_chromosome = -1

with open(filename, 'r') as file:
    current_generation = -1
    for line in file:

        if match := re.match(r"Average Fitness = ([\d.]+)", line):
            average_fitness.append(float(match.group(1)))
        
        if match := re.match(r"Best Ever Fitness = ([\d.]+)", line):
            best_fitness.append(float(match.group(1)))

x_values = range(len(average_fitness))
plt.plot(x_values, average_fitness, marker='o', linestyle='-', label="Average Fitness", color="blue")
plt.plot(x_values, best_fitness, marker='s', linestyle='--', label="Best Fitness", color="red")
plt.title("Fitness Over Generations")
plt.xlabel("Generation")
plt.ylabel("Fitness")
plt.legend()
plt.grid()
timestamp_str = timestamp.strftime("%Y-%m-%d-%H%M%S") 
output_path = os.path.join("output", f"fitness_{timestamp_str}.jpg")
plt.savefig(output_path, dpi=300)
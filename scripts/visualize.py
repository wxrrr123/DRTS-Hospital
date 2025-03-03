import re
import os
import sys
import glob
import matplotlib.pyplot as plt
from datetime import datetime

pattern = r"(\d{8}_\d{6})\.txt"
search_path = os.path.join("output", "*.txt")

file_times = [
    (file, datetime.strptime(m.group(1), "%Y%m%d_%H%M%S"))
    for file in glob.glob(search_path)
    if (m := re.search(pattern, os.path.basename(file)))
]

if not file_times:
    print("Cannot find any output file.", file=sys.stderr)
    sys.exit(1)

filename, timestamp = max(file_times, key=lambda x: x[1])  # Find the latest file
print(f"Visualizing {filename}...")

average_fitness, best_fitness = [], []
with open(filename, 'r') as file:
    for line in file:
        if (m := re.match(r"Average Fitness = ([\d.]+)", line)):
            average_fitness.append(float(m.group(1)))
        elif (m := re.match(r"Best Ever Fitness = ([\d.]+)", line)):
            best_fitness.append(float(m.group(1)))

plt.plot(range(len(average_fitness)), average_fitness, 'o-', label="Average Fitness", color="blue")
plt.plot(range(len(best_fitness)), best_fitness, 's--', label="Best Fitness", color="red")
plt.title("Fitness Over Generations")
plt.xlabel("Generation")
plt.ylabel("Fitness")
plt.legend()
plt.grid()

output_path = os.path.join("output", f"{timestamp.strftime('%Y%m%d_%H%M%S')}.jpg")
plt.savefig(output_path, dpi=300)
print(f"Visualization saved to {output_path}.")

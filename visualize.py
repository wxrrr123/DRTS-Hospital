import re
import matplotlib.pyplot as plt

filename = "result.txt"
generations = []
average_fitness = []
highest_fitness = -float('inf')
highest_generation = -1
highest_chromosome = -1

gene_to_time = {
    "00": 40,
    "01": 75,
    "10": 110,
    "11": 150
}

with open(filename, 'r') as file:
    current_generation = -1
    for line in file:

        if match := re.match(r">>> GENERATION (\d+) <<<", line):
            current_generation = int(match.group(1))
            generations.append(current_generation)

        elif match := re.match(r"Average Fitness = ([\d.]+)", line):
            average_fitness.append(float(match.group(1)))

        elif match := re.match(r"Chromosome (\d+) => ([01 ]+) Fitness = ([\d.]+)", line):
            chromosome_num = int(match.group(1))
            chromosome_sequence = match.group(2).replace(" ", "")
            fitness = float(match.group(3))

            if fitness > highest_fitness:
                highest_fitness = fitness
                highest_generation = current_generation
                highest_chromosome = chromosome_num
                highest_chromosome_sequence = chromosome_sequence

# decode
start_time = 10 * 60 
decoded_times = []

for i in range(0, len(highest_chromosome_sequence), 8): 
    group_time = start_time 
    group_decoded_times = [] 

    for j in range(i, i + 8, 2): 
        if j + 2 <= len(highest_chromosome_sequence): 
            gene = highest_chromosome_sequence[j:j + 2]
            if gene in gene_to_time:
                group_time += gene_to_time[gene]
                group_decoded_times.append(group_time)

    decoded_times.append([f"{t // 60:02}:{t % 60:02}" for t in group_decoded_times])
                
print(f"Highest Fitness: {highest_fitness}")
print(f"Generation: {highest_generation}")
print(f"Chromosome: {highest_chromosome}")
for idx, group in enumerate(decoded_times):
    print(f"Route {idx + 1}: {group}")

plt.plot(generations, average_fitness, marker='o', linestyle='-')
plt.title("Average Fitness Over Generations")
plt.xlabel("Generation")
plt.ylabel("Average Fitness")
plt.grid()
plt.savefig("fitness.jpg", dpi=300)

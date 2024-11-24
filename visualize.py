import re
import matplotlib.pyplot as plt

filename = "result.txt"
generations = []
average_fitness = []
highest_fitness = -float('inf')
highest_generation = -1
highest_chromosome = -1

with open(filename, 'r') as file:
    current_generation = -1
    for line in file:

        if match := re.match(r">>> GENERATION (\d+) <<<", line):
            current_generation = int(match.group(1))
            generations.append(current_generation)

        elif match := re.match(r"Average Fitness = ([\d.]+)", line):
            average_fitness.append(float(match.group(1)))

        elif match := re.match(r"Chromosome (\d+) .* Fitness = ([\d.]+)", line):
            chromosome_num = int(match.group(1))
            fitness = float(match.group(2))
    
            if fitness > highest_fitness:
                highest_fitness = fitness
                highest_generation = current_generation
                highest_chromosome = chromosome_num
                
print(f"Highest Fitness: {highest_fitness}")
print(f"Generation: {highest_generation}")
print(f"Chromosome: {highest_chromosome}")

plt.plot(generations, average_fitness, marker='o', linestyle='-')
plt.title("Average Fitness Over Generations")
plt.xlabel("Generation")
plt.ylabel("Average Fitness")
plt.grid()
plt.savefig("fitness.jpg", dpi=300)

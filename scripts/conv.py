import re
import os

def analyze_best_ever_in_folder(folder_path, generations_no_improve=500):
    best_ever_pattern = re.compile(r"Best Ever Fitness = ([\d.]+)")
    generation_pattern = re.compile(r">>> GENERATION (\d+) <<<")
    time_consumed_pattern = re.compile(r"Time Consumed = ([\d.]+) min")
    best_chrom_pattern = re.compile(r"Best Chrom = ([01 ]+)")

    all_best_chroms = []

    for file_name in os.listdir(folder_path):
        if file_name.endswith(".txt"):
            file_path = os.path.join(folder_path, file_name)
            print(f"File: {file_name}")
            
            best_ever_first_gen = None
            best_ever_value = None
            last_improvement_gen = None
            last_improvement_time = None
            best_chrom = None
            current_gen = None
            consecutive_no_improve = 0

            with open(file_path, 'r') as file:
                lines = file.readlines()

            for line in lines:
                # Match generation number
                gen_match = generation_pattern.search(line)
                if gen_match:
                    current_gen = int(gen_match.group(1))
                
                # Match Best Ever Fitness
                best_match = best_ever_pattern.search(line)
                if best_match:
                    current_best = float(best_match.group(1))
                    if best_ever_value is None or current_best < best_ever_value:
                        best_ever_value = current_best
                        best_ever_first_gen = current_gen
                        last_improvement_gen = current_gen
                        consecutive_no_improve = 0
                    elif current_best == best_ever_value:
                        consecutive_no_improve += 1
                    else:
                        consecutive_no_improve = 0

                # Match Time Consumed
                time_match = time_consumed_pattern.search(line)
                if time_match and current_gen == last_improvement_gen:
                    last_improvement_time = float(time_match.group(1))

                # Match Best Chrom
                chrom_match = best_chrom_pattern.search(line)
                if chrom_match:
                    best_chrom = chrom_match.group(1).replace(" ", "")

                if consecutive_no_improve >= generations_no_improve:
                    if best_chrom and best_chrom not in all_best_chroms:
                        all_best_chroms.append(best_chrom)
                    print(f"Best Ever Fitness: {best_ever_value}")
                    print(f"Time Consumed: {last_improvement_time} min")
                    print(f"First Appeared At Generation: {best_ever_first_gen}")
                    print(f"Last Improvement Generation: {last_improvement_gen}")
                    break
            else:
                if best_chrom and best_chrom not in all_best_chroms:
                    all_best_chroms.append(best_chrom)
                print(f"Best Ever Fitness: {best_ever_value}")
                print(f"Time Consumed: {last_improvement_time} min")
                print(f"First Appeared At Generation: {best_ever_first_gen}")
                print(f"Last Improvement Generation: {last_improvement_gen}")
            print("-" * 50)

    print("All Best Chroms:")
    print(",\n".join(f'"{chrom}"' for chrom in all_best_chroms))

folder_path = "output"
analyze_best_ever_in_folder(folder_path)
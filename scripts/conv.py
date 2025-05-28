import re
import os

def analyze_generation_in_folder(folder_path, target_gen):
    best_ever_pattern = re.compile(r"Best Ever Fitness = ([\d.]+)")
    generation_pattern = re.compile(r">>> GENERATION (\d+) <<<")
    time_consumed_pattern = re.compile(r"Time Consumed = ([\d.]+) min")
    best_result_test_pattern = re.compile(r">>> Best Result Test: ([\d.]+)")

    for file_name in os.listdir(folder_path):
        if file_name.endswith(".txt"):
            file_path = os.path.join(folder_path, file_name)
            print(f"File: {file_name}")
            
            with open(file_path, 'r') as file:
                lines = file.readlines()

            current_gen = None
            found_target_gen = False
            target_gen_best_ever_value = None
            best_ever_values_by_gen = {}  # To track best ever values by generation
            
            # First pass: Find the best ever value at target generation
            for line in lines:
                gen_match = generation_pattern.search(line)
                if gen_match:
                    current_gen = int(gen_match.group(1))
                
                if current_gen == target_gen:
                    found_target_gen = True
                    best_match = best_ever_pattern.search(line)
                    if best_match:
                        target_gen_best_ever_value = float(best_match.group(1))
            
            if not found_target_gen or target_gen_best_ever_value is None:
                print(f"Generation {target_gen} not found or no Best Ever Fitness value found.")
                print("-" * 50)
                continue
            
            # Second pass: Find when this best ever value first appeared
            current_gen = None
            first_appeared_gen = None
            time_at_target_gen = None
            
            for line in lines:
                gen_match = generation_pattern.search(line)
                if gen_match:
                    current_gen = int(gen_match.group(1))
                
                # Record best ever value for each generation
                best_match = best_ever_pattern.search(line)
                if best_match and current_gen is not None:
                    best_ever_value = float(best_match.group(1))
                    best_ever_values_by_gen[current_gen] = best_ever_value
                    
                    # If this matches our target value and we haven't found the first appearance yet
                    if best_ever_value == target_gen_best_ever_value and first_appeared_gen is None:
                        first_appeared_gen = current_gen
                
                # Capture time at target generation
                if current_gen == target_gen:
                    time_match = time_consumed_pattern.search(line)
                    if time_match:
                        time_at_target_gen = time_match.group(1)
            
            # Check for Best Result Test in the file
            best_result_value = None
            for line in lines:
                best_result_match = best_result_test_pattern.search(line)
                if best_result_match:
                    best_result_value = best_result_match.group(1)
            
            # Output results
            print(f"Best Ever Fitness at Generation {target_gen}: {target_gen_best_ever_value}")
            if time_at_target_gen:
                print(f"Time Consumed at Generation {target_gen}: {time_at_target_gen} min")
            if first_appeared_gen:
                print(f"First Appeared At Generation: {first_appeared_gen}")
            if best_result_value:
                print(f"Best Result Test: {best_result_value}")
            
            print("-" * 50)

# Example usage
folder_path = "useful_output/ocba_300_12"
target_gen = int(input("Enter the generation number to analyze: "))
analyze_generation_in_folder(folder_path, target_gen)
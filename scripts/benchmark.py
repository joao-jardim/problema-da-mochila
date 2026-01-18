import subprocess
import time
import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm
import glob

# Configuration
SOLVER_PATH = "./solver"
OUTPUT_DIR = "output"
INPUT_DIR = "input"
RESULTS_FILE = "results.csv"

# Experiment Params
# "Gere diversas combinações ... comportamento assintótico"
# We will vary N. W and V can scale with N or be fixed large enough.
# Let's fix W and V to be somewhat constraining but large enough to fit many items.
# Or better: W = n * 10, V = n * 10 to keep 'tightness' constant.

# Small scale for testing (and for exponential algos like Backtracking)
Ns_small = [5, 10, 15, 20, 25] 
# Larger scale for DP/BB if they are efficient enough (BB worst case is bad too)
Ns_large = [50, 100, 200] 

# Note: Backtracking O(2^n) will die around n=40-50 usually.
TARGET_NS = [4, 8, 12, 16, 20, 24] # Conservative start for exponential algos

def run_solver(strategy, input_file):
    try:
        # Capture stdout to parse execution time and value if needed from C++ output
        # or rely on the tool to measure time.
        # The C++ tool prints "Max Value: ..." and "Time: ..."
        result = subprocess.run(
            [SOLVER_PATH, strategy, input_file], 
            capture_output=True, 
            text=True, 
            timeout=60 # 1 minute timeout per instance
        )
        output = result.stdout.splitlines()
        max_value = float(output[0].split(": ")[1])
        exec_time = float(output[1].split(": ")[1].replace("s", ""))
        return max_value, exec_time
    except subprocess.TimeoutExpired:
        return None, None
    except Exception as e:
        print(f"Error running {strategy} on {input_file}: {e}")
        return None, None

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    os.makedirs(INPUT_DIR, exist_ok=True)
    
    results = []
    
    # 10 instances for each configuration
    NUM_INSTANCES = 10
    
    print("Generating instances and running benchmarks...")
    
    for n in TARGET_NS:
        print(f"Processing N={n}")
        # Scale W and V so it's not trivial
        W = n * 20
        V = n * 20
        
        for i in range(NUM_INSTANCES):
            instance_name = f"n{n}_i{i}"
            input_path = os.path.join(INPUT_DIR, f"{instance_name}.txt")
            
            # Generate if not exists (or overwrite)
            subprocess.run(["python3", "scripts/generator.py", str(n), str(W), str(V), input_path])
            
            # Run all 3 algorithms
            for strategy in ["BT", "BB", "DP"]:
                # For larger N, maybe skip BT if it gets too slow
                if n > 25 and strategy == "BT": 
                    continue
                    
                val, t = run_solver(strategy, input_path)
                
                if val is not None:
                    results.append({
                        "n": n,
                        "W": W,
                        "V": V,
                        "instance": i,
                        "strategy": strategy,
                        "max_value": val,
                        "time": t
                    })
    
    # Save results
    df = pd.DataFrame(results)
    df.to_csv(RESULTS_FILE, index=False)
    print(f"Results saved to {RESULTS_FILE}")
    
    # Plotting
    if not df.empty:
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x="n", y="time", hue="strategy", style="strategy", markers=True)
        plt.title("Execution Time vs Number of Items")
        plt.xlabel("Number of Items (N)")
        plt.ylabel("Time (seconds)")
        plt.yscale("log")
        plt.grid(True)
        plt.savefig(os.path.join(OUTPUT_DIR, "time_comparison.png"))
        print("Plot saved to output/time_comparison.png")

        # Statistical Analysis
        print("\n--- Statistical Analysis ---")
        try:
            import scipy.stats as stats
            # Pivot data for statistical test: rows=instances, cols=strategies, values=time
            # We need to do this per N, presumably.
            unique_ns = df['n'].unique()
            for n_val in unique_ns:
                print(f"\nAnalyzing for N={n_val}:")
                subset = df[df['n'] == n_val]
                pivot = subset.pivot(index='instance', columns='strategy', values='time')
                
                # Check if we have all strategies
                if pivot.shape[1] == 3:
                    stat, p = stats.friedmanchisquare(pivot['BT'], pivot['BB'], pivot['DP'])
                    print(f"Friedman Test: statistic={stat:.4f}, p-value={p:.4e}")
                    if p < 0.05:
                        print("Result: Significant difference found.")
                    else:
                        print("Result: No significant difference found (Statistical Tie).")
                else:
                    print("Skipping statistical test (not all algorithms ran).")
                    
        except ImportError:
            print("scipy not installed. Skipping statistical tests.")


if __name__ == "__main__":
    main()

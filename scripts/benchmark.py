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
        # Select executable based on strategy
        if strategy == "BT":
            executable = "./solver_bt"
        elif strategy == "BB":
            executable = "./solver_bb"
        elif strategy == "DP":
            executable = "./solver_dp"
        else:
            return None, None, None

        # Use /usr/bin/time -v to measure memory
        # Output is directed to stderr, so we capture that.
        cmd = ["/usr/bin/time", "-v", executable, input_file]
        
        result = subprocess.run(
            cmd, 
            capture_output=True, 
            text=True, 
            timeout=60
        )
        
        # Parse stdout for logic output (Max Value)
        output = result.stdout.splitlines()
        max_value = float(output[0].split(": ")[1])
        # We can still use internal time, or use time command's time. 
        # Using internal time from stdout as before for consistency.
        exec_time = float(output[1].split(": ")[1].replace("s", ""))

        # Parse stderr for memory (Maximum resident set size)
        stderr_output = result.stderr.splitlines()
        memory_kb = 0
        for line in stderr_output:
            if "Maximum resident set size" in line:
                # Format: "Maximum resident set size (kbytes): 1234"
                memory_kb = float(line.split(": ")[1].strip())
                break

        return max_value, exec_time, memory_kb

    except subprocess.TimeoutExpired:
        return None, None, None
    except Exception as e:
        print(f"Error running {strategy} on {input_file}: {e}")
        return None, None, None

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    os.makedirs(INPUT_DIR, exist_ok=True)
    
    results = []
    
    # 30 instances for better statistical consistency 
    NUM_INSTANCES = 30
    
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
                    
                val, t, mem = run_solver(strategy, input_path)
                
                if val is not None:
                    results.append({
                        "n": n,
                        "W": W,
                        "V": V,
                        "instance": i,
                        "strategy": strategy,
                        "max_value": val,
                        "time": t,
                        "memory_kb": mem
                    })
    
    # Save results
    df = pd.DataFrame(results)
    df.to_csv(RESULTS_FILE, index=False)
    print(f"Results saved to {RESULTS_FILE}")
    
    # Plotting Time
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

        # Plotting Memory
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x="n", y="memory_kb", hue="strategy", style="strategy", markers=True)
        plt.title("Memory Usage vs Number of Items")
        plt.xlabel("Number of Items (N)")
        plt.ylabel("Memory (KB)")
        plt.grid(True)
        plt.savefig(os.path.join(OUTPUT_DIR, "memory_comparison.png"))
        print("Plot saved to output/memory_comparison.png")

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
                
                # Check time differences
                pivot_time = subset.pivot(index='instance', columns='strategy', values='time')
                if pivot_time.shape[1] == 3:
                    stat, p = stats.friedmanchisquare(pivot_time['BT'], pivot_time['BB'], pivot_time['DP'])
                    print(f"Time Friedman Test: p-value={p:.4e}")
                
                # Check memory differences (optional, usually obvious)
                pivot_mem = subset.pivot(index='instance', columns='strategy', values='memory_kb')
                if pivot_mem.shape[1] == 3:
                     stat, p = stats.friedmanchisquare(pivot_mem['BT'], pivot_mem['BB'], pivot_mem['DP'])
                     print(f"Memory Friedman Test: p-value={p:.4e}")

        except ImportError:
            print("scipy not installed. Skipping statistical tests.")

if __name__ == "__main__":
    main()

import random
import sys
import os

def generate_instance(n, W_max, V_max, output_file):
    with open(output_file, 'w') as f:
        # First line: W V
        # W and V should be roughly proportional to n * avg_weight * something,
        # or just fixed large numbers as per assignment requirements testing asymptotic behavior.
        # Requirements say: "Gere diversas combinações de quantidade de itens e peso e volume suportados pela mochila"
        
        # Let's assume W_max and V_max are passed as args.
        f.write(f"{W_max} {V_max}\n")
        
        for _ in range(n):
            # w, l (volume), v (value)
            w = random.randint(1, int(W_max/2)) if n < 5 else random.randint(1, 50)
            l = random.randint(1, int(V_max/2)) if n < 5 else random.randint(1, 50)
            v = random.randint(1, 100)
            f.write(f"{w}\t{l}\t{v}\n")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python generator.py <n> <W> <V> <output_file>")
        sys.exit(1)
        
    n = int(sys.argv[1])
    W = int(sys.argv[2])
    V = int(sys.argv[3])
    output_file = sys.argv[4]
    
    generate_instance(n, W, V, output_file)

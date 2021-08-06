#!/usr/bin/python3

import sys
import subprocess

def run_mutex(exe, threads):
    print(f"running {exe} {threads}")
    result = subprocess.run([exe, threads], capture_output=True)
    lines = result.stdout.split(b"\n")
    numbers = [int(line.split(b": ")[1]) for line in lines[:-1]]
    throughput = int(numbers[0])
    hist = numbers[1:]

    mean = sum(hist) / len(hist)
    ss = sum([(v - mean)**2 for v in hist])
    variance = ss / len(hist)

    print(f"throughput: {throughput}, variance: {variance}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: ./analyze.py [PATH] [THREADS]")
        exit(1)
    exe = sys.argv[1]
    threads = sys.argv[2]
    run_mutex(exe, threads)
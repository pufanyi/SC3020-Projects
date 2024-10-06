import matplotlib.pyplot as plt
import seaborn as sns

# Data for plotting
buffer_size = [1, 10, 50, 100, 250, 500]
time_ms = [39, 39, 34, 29, 13, 9]
io = [6878, 6352, 5214, 4060, 879, 0]

# Lineplot for Time (ms) vs Buffer Size
plt.figure(figsize=(10, 6))
sns.lineplot(x=buffer_size, y=time_ms, marker='o', label="B+ Tree")
plt.axhline(y=23, color='r', linestyle='--', label="Linear scan (23 ms)")
plt.title("Time (ms) vs Buffer Size")
plt.xlabel("Buffer Size")
plt.ylabel("Time (ms)")
plt.legend()
plt.grid(True)
plt.savefig("time_vs_buffer_size.png")

# Lineplot for IO vs Buffer Size
plt.figure(figsize=(10, 6))
sns.lineplot(x=buffer_size, y=io, marker='o', label="B+ Tree")
plt.axhline(y=293, color='r', linestyle='--', label="Linear scan (293 IO)")
plt.title("IO vs Buffer Size")
plt.xlabel("Buffer Size")
plt.ylabel("IO")
plt.legend()
plt.grid(True)
plt.savefig("io_vs_buffer_size.png")

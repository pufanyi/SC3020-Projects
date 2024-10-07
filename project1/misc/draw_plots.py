import matplotlib.pyplot as plt
import seaborn as sns

# Data for plotting
buffer_size = [1, 100, 250, 500]
time_ms = [39, 27, 13, 9]
index_io = [66, 57, 41, 0]
data_block_io = [6812, 4003, 838, 0]

# Lineplot for Time (ms) vs Buffer Size
plt.figure(figsize=(10, 6))
sns.lineplot(x=buffer_size, y=time_ms, marker='o', label="B+ Tree")
plt.axhline(y=23, color='r', linestyle='--', label="Linear scan (23 ms)")
plt.title("Time (ms) vs Buffer Size", fontsize=25)
plt.xlabel("Buffer Size", fontsize=20)
plt.ylabel("Time (ms)", fontsize=20)
plt.legend()
plt.grid(True)
plt.savefig("time_vs_buffer_size.pdf")

# Lineplot for Index IO vs Buffer Size
plt.figure(figsize=(10, 6))
sns.lineplot(x=buffer_size, y=index_io, marker='o', label="B+ Tree")
# plt.axhline(y=293, color='r', linestyle='--', label="Linear scan (293 IO)")
plt.title("IO (Index) vs Buffer Size", fontsize=25)
plt.xlabel("Buffer Size", fontsize=20)
plt.ylabel("IO (Index)", fontsize=20)
plt.legend()
plt.grid(True)
plt.savefig("index_io_vs_buffer_size.pdf")


# Lineplot for BLock IO vs Buffer Size
plt.figure(figsize=(10, 6))
sns.lineplot(x=buffer_size, y=data_block_io, marker='o', label="B+ Tree")
plt.axhline(y=293, color='r', linestyle='--', label="Linear scan (293 IO)")
plt.title("IO (Data) vs Buffer Size", fontsize=25)
plt.xlabel("Buffer Size", fontsize=20)
plt.ylabel("IO (Data)", fontsize=20)
plt.legend()
plt.grid(True)
plt.savefig("block_io_vs_buffer_size.pdf")

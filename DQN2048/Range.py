import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LinearSegmentedColormap

# Đọc dữ liệu
rewards = []
with open('log.txt') as f:
    for line in f:
        try:
            rewards.append(float(line.strip()))
        except:
            pass

# Tính độ thay đổi reward (delta)
window = 100
avg_rewards = []
for i in range(len(rewards)):
    avg = sum(rewards[max(0, i - window):i + 1]) / (i - max(0, i - window) + 1)
    avg_rewards.append(avg)
deltas = [0] + [avg_rewards[i] - avg_rewards[i - 1] for i in range(1, len(avg_rewards))]

# Normalize và đảo chiều: reward tăng → 0 (xanh), giảm → 1 (đỏ)
min_delta = min(deltas)
max_delta = max(deltas)
delta_norm = [max(min(d, 15), -15) for d in deltas]

# Colormap: xanh → vàng → đỏ
cmap = LinearSegmentedColormap.from_list("smooth", ["red", "yellow", "green"])

# Vẽ scatter
plt.figure(figsize=(10, 5))
plt.scatter(range(len(rewards)), rewards, c=delta_norm, cmap=cmap, s=2, alpha=0.8)
plt.xlabel("Số trận")
plt.ylabel("Reward")
plt.title("Reward với màu phản ánh xu hướng tăng/giảm")
plt.grid(True, linewidth=0.4)
plt.colorbar(label="Reward giảm dần →")
plt.tight_layout()
plt.show()

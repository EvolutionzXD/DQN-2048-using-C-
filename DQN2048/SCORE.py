import matplotlib.pyplot as plt

rewards = []
with open('log.txt') as f:
    for line in f:
        try:
            rewards.append(float(line.strip()))
        except:
            pass

# Tính reward trung bình động (moving average)
window = 100
avg_rewards = []
for i in range(len(rewards)):
    avg_rewards.append(sum(rewards[max(0, i - window):i + 1]) / (i - max(0, i - window) + 1))

plt.figure(figsize=(10, 5))
plt.scatter(range(len(rewards)), rewards, label='Reward mỗi trận', alpha=0.7, s=0.6)
plt.xlabel('Số trận')
plt.ylabel('Reward')
plt.title('Biểu đồ reward trong quá trình train')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

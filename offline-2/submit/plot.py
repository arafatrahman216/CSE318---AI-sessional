import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
data = pd.read_csv('2105118.csv')  # Replace with your actual filename
print(data.head())  # Display the first few rows of the dataframe

data= data.head(10)

# Extract the relevant columns for plotting
graph_names = data['Name']
# Update the algorithm names to match the actual column names in the CSV file
algorithms = ['Randomized-1', 'Greedy-1', 'Semi-greedy-1', 
              'Local-1', 'GRASP-1']
known_best = data['Known best']

# Create a figure
plt.figure(figsize=(12, 8))

# Set the bar width
bar_width = 0.15
index = np.arange(len(graph_names))

# Plot bars for each algorithm
for i, algo in enumerate(algorithms):
    plt.bar(index + i*bar_width, data[algo], bar_width, label=algo)

# Plot the known best solution as a horizontal line
plt.axhline(y=known_best[0], color='r', linestyle='--', label='Known Best Solution')

# Customize the plot
plt.xlabel('Graph Instances', fontsize=12)
plt.ylabel('Cut Weight', fontsize=12)
plt.title('Max-Cut Algorithm Performance Comparison', fontsize=14)
plt.xticks(index + bar_width*2, graph_names, rotation=45)
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
plt.grid(True, axis='y', linestyle='--', alpha=0.7)

# Adjust layout
plt.tight_layout()

# Show the plot
plt.show()

plt.savefig('max_cut_comparison.png', bbox_inches='tight')
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import argparse

# Set up argument parser
parser = argparse.ArgumentParser(description='Plot motif counting results from a CSV file.')
parser.add_argument('csv_file', type=str, help='The CSV file containing the results.')

# Parse arguments
args = parser.parse_args()

# Read the CSV file
df = pd.read_csv(args.csv_file)

# Plotting the data
plt.figure(figsize=(10, 6))
plot = sns.barplot(x="Threads", y="Time (seconds)", data=df)
plot.set_title("Motif Counting Time vs. Number of Threads")
plt.xticks(rotation=45)

# Generate the PDF filename based on the CSV filename
pdf_filename = args.csv_file.replace('.csv', '_results.pdf')

# Save the plot to a PDF file
plt.savefig(pdf_filename)

print(f"Plot saved to {pdf_filename}")



import csv

x = []
y = []
z = []

tidal_data = []
with open('data/tidal_data.csv', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        tidal_data.append([row[0], row[1]])

with open('data/grohn26082021_raw.csv', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        for tidal in tidal_data:
            if tidal[0] == row[1][:-3]:
                # time.append(row[1][:-3])
                x.append(float(row[4]))
                y.append(float(row[3]))
                z.append(float(row[2]) + float(tidal[1]))

                break

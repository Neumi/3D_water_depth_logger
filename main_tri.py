import numpy as np
import matplotlib as mpl
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import csv
import matplotlib.tri as tri
import math

'''
x = []
y = []
z = []
with open('data/grohn26082021_raw.csv', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        if 0.2 < float(row[2]) < 2.5:
            x.append(float(row[4]))
            y.append(float(row[3]))
            z.append(float(row[2]))

'''
x = []
y = []
z = []
time = []

tidal_data = []
with open('data/tidal_data.csv', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        tidal_data.append([row[0], row[1]])

with open('data/grohn26082021_raw.csv', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        for tidal in tidal_data:
            if tidal[0] == row[1][:-3] and 0.3 < float(row[2]) < 2:
                # time.append(row[1][:-3])
                x.append(float(row[4]))
                y.append(float(row[3]))
                z.append(4 - (float(tidal[1]) - float(row[2])))

                break

print(z)

latmin = min(x)
latmax = max(x)
lngmin = min(y)
lngmax = max(y)

npts = 150
ngridx = 150
ngridy = 150

fig, ax = plt.subplots()

xi = np.linspace(latmin, latmax, ngridx)
yi = np.linspace(lngmin, lngmax, ngridy)

triang = tri.Triangulation(x, y)
interpolator = tri.LinearTriInterpolator(triang, z)
Xi, Yi = np.meshgrid(xi, yi)
zi = interpolator(Xi, Yi)

ax.contour(xi, yi, zi, levels=20, linewidths=0.2, colors='k')
cntr1 = ax.contourf(xi, yi, zi, levels=20, cmap="gist_rainbow")  # RdBu_r

fig.colorbar(cntr1, ax=ax, orientation="horizontal").set_label("Tiefe in m")
ax.plot(x, y, 'ko', ms=0.5)
ax.set(xlim=(latmin, latmax), ylim=(lngmin, lngmax))
ax.set_title("Tiefenkarte Grohner Yachthafen")
ax.ticklabel_format(useOffset=False)

ax.set_aspect(1 / math.cos(math.radians(60.0)))
plt.savefig("depthmap.png", dpi=220)
plt.show()

import matplotlib.pyplot as plt
import csv

normVal = []
smthVal = []
powrVal = []
spline  = []
line  = []
curve   = []

with open('normal.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        normVal.append(row[0]);

with open('smooth.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        smthVal.append(row[0]);

with open('line.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        line.append(row[0]);


with open('curve.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        curve.append(row[0]);

with open('spline.csv', 'rb') as f:
    reader = csv.reader(f)
    for row in reader:
        spline.append(row[0]);


# plt.plot(normVal)
# plt.xlabel('Time')
# plt.ylabel('Delay')

plt.plot(spline)
plt.xlabel('Time')
plt.ylabel('Delay')

# plt.plot(smthVal)
# plt.xlabel('Time')
# plt.ylabel('Delay')

plt.plot(curve)
plt.xlabel('Time')
plt.ylabel('Delay')

plt.plot(line)
plt.xlabel('Time')
plt.ylabel('Delay')


plt.show()

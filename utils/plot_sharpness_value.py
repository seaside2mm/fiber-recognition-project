import matplotlib.pyplot as plt

def Normalization(x):
    return [(float(i)-min(x))/float(max(x)-min(x)) for i in x]

names = []
blur_values = []
lines = []

#read all sharpness value and compute the mean value
with open("/Users/kubota/Documents/program/project/cmake-build-debug/output.txt",'r') as f:
    for line in f.readlines():
        lines.append(line)
        name, blur_value = line.split()
        names.append(name)
        blur_values.append(float(blur_value))
xs = [i for i in range(len(names))]
standard_data = Normalization(blur_values)
plt.plot(xs, standard_data,'ro')
min_x = min(blur_values)
min_index = blur_values.index(min_x)
print(("The most clear one is: %s, value is: %f") % (names[min_index], min_x))
plt.show()

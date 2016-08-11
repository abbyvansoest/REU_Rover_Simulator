from matplotlib import pyplot as plt
import numpy as np
import sys

def running_avg(data, avg_width):
    '''
    Calculates the properly scaled running average.
    Used np.convolve in the past, but it returns
    values on a different scale, since it is not
    technically a running average, so I coded up one quick
    '''
    out = np.zeros(len(data)-avg_width)
    for i in range(len(data)-avg_width):
        out[i] = np.average(data[i:i+avg_width])
    return out

# Begin Preprocessing
data = []
for f_name in sys.argv[1:]:
    f = open(f_name)
    data.append(np.array([float(line) for line in f.readlines()]))

# Specifies how many samples to consider in a section of the running average
avg_width = 150
# End preprocessing

###################################################
### This section plots the smoothed data, then  ###
### calculates the average of the data,         ###
### smoothes it, and plots the smoothed avg.    ###
###################################################
'''for d in data:
    smoothed_d = running_avg(d, avg_width)
    plt.plot(smoothed_d)

# Average
averaged = np.average(data, axis=0)
smoothed_d = running_avg(averaged, avg_width)
plt.plot(smoothed_d)

# Construct legend (based on the filenames. Will probably require reworking for whatever filename pattern is given.)
legend = sys.argv[1:]
legend = ["Trial " + line[2] for line in legend] # Comment out this line if you just want the filenames to be on the legend
legend.append("Average")
plt.legend(legend)'''

###################################################
### This plots the averages from different      ###
### types of runs togeter, by first calculating ###
### the averages of the different types of data ###
### sets, then smoothing them with the running  ###
### average and plotting.                       ###
###################################################

average_one = np.average(data[0:5], axis=0)
average_five = np.average(data[5:10], axis=0)
'''average_five = np.average(data[10:15], axis=0)'''

'''smoothed_d = running_avg(average_d, avg_width)'''
smoothed_one = running_avg(average_one, avg_width)
smoothed_five = running_avg(average_five, avg_width)

'''plt.plot(smoothed_d)'''
plt.plot(smoothed_one)
plt.plot(smoothed_five)
legend = ["One", "Five"]
'''plt.legend(legend)'''

# Label axis and plot
plt.xlabel("Generation")
plt.ylabel("worlds complete")
plt.title("worlds complete per epoch in one-step and five-step projected domains")

plt.show()



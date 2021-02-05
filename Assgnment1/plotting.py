import matplotlib.pyplot as plt
import time
import sys
import subprocess

percentage = [1,5,10,25,50,90]

fileInput = sys.argv[1]

aprioriTime, fpTreeTime = [], []

for percent in percentage:
    startAp = time.time()
    subprocess.run("./aprOut "+fileInput+" "+str(percent)+" "+"apOut.txt > apOut.txt", shell = True)
    aprioriTime.append(time.time()-startAp)

    startFP = time.time()
    subprocess.run("./fpOut "+fileInput+" "+str(percent)+" "+"fpTOut.txt > fpTOut.txt", shell = True)
    fpTreeTime.append(time.time()-startFP)

fig, ax = plt.subplots()
ax.set_xlabel("Support threshold in percentage (%)")
ax.set_ylabel("Runtime of algorithms (secs)")
ax.plot(percentage, aprioriTime, color = 'red', label = 'Apriori Runtime')
ax.plot(percentage, fpTreeTime, color = 'blue', label = 'FpTree Runtime')
ax.legend()
# plt.show()
plt.savefig("Plot.png")

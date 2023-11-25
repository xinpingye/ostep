#! /usr/bin/env python3
import random
joblist = []
random.seed(100)
for jobnum in range(0,7):
    runtime = int(random.random() * 10) + 1
    joblist.append([jobnum, runtime])
    print('  Job', jobnum, '( length = ' + str(runtime) + ' )')

def print_1():
    for i in range(0,7):
        print(i,'times run ','jobnum ',joblist[i][0],'and length ',joblist[i][1])

#joblist = sorted(joblist, key=lambda s: s[1])

#for i in range(0,len(joblist)):
#    print('jobnum:',joblist[i][0],'length:',joblist[i][1])

#for i in range(0,len(joblist)):
#    print(i,'times run ','jobnum ',joblist[i][0],'and length ',joblist[i][1])

time_slice = 2
time = 0
end = False
while 1:
    #print("**********")
    for i in range(0,7):
        #print("^^^^^^^^^")
        #print_1()
        #print("^^^^^^^^^")
        if  joblist[i][1] > 0:
            if joblist[i][1] <= time_slice:
                print(time," times run",'jobnum ', joblist[i][0],'and length ',joblist[i][1])
                # if time_slice is greater than the neead of this process this time,just give up the other part of time_slice
                time += joblist[i][1]
                joblist[i][1] = 0
                continue
            else:
                print(time," times run","jobnum ", joblist[i][0],'and length ',time_slice)
                joblist[i][1] -= time_slice
                time += time_slice
                continue
        else:
            if  i == 6:
                end = True
    if  end:
        break
print(time," time ","end")

print_1()

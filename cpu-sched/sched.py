#! /usr/bin/env python3
import random
joblist = []
random.seed(100)
for jobnum in range(0,7):
    runtime = int(random.random() * 10) + 1
    joblist.append([jobnum, runtime])
    print('  Job', jobnum, '( length = ' + str(runtime) + ' )')

joblist = sorted(joblist, key=lambda s: s[1])

for i in range(0,len(joblist)):
    print('jobnum:',joblist[i][0],'length:',joblist[i][1])

for i in range(0,len(joblist)):
    print(i,'times run ','jobnum ',joblist[i][0],'and length ',joblist[i][1])

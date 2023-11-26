#! /usr/bin/env python

# fix three queue
queue = {}
queue[0] = []
queue[1] = []
queue[2] = []

# fix joblist
joblist = {}
joblist[0] = {"start_time":0,"time_len":30}
joblist[1] = {"start_time":0,"time_len":20}
joblist[2] = {"start_time":10,"time_len":40}

# keep alloctime >= time_slice

#fix time_slice 
time_slice = 10

#fix alloctime
alloctime = 10

# jobnum && queue && timeleft && time_slice left && alloctime left 
jobctrl = {}
jobctrl[0] = {"queuenum":2,"timeleft":joblist[0]["time_len"],"time_slice_left":time_slice,"alloc_time_left":alloctime}
jobctrl[1] = {"queuenum":2,"timeleft":joblist[1]["time_len"],"time_slice_left":time_slice,"alloc_time_left":alloctime}
jobctrl[2] = {"queuenum":2,"timeleft":joblist[2]["time_len"],"time_slice_left":time_slice,"alloc_time_left":alloctime}

current_time = 0

queue_priority = [2,1,0]

queue_top = 2

job_left = len(joblist)

def print_queue():
    for i in range(0,3):
        for j in range(0,len(queue[i])):
            print("queue:",i," has job:",queue[i][j],"in position:",j)

while job_left:
    #print_queue()
    # push job into queue
    for i in range(0,len(joblist)):
        if  current_time == joblist[i]["start_time"]:
            queue[queue_top].append(i)
    queue_curr = -1
    for i in queue_priority:
        #print("i:",i)
        #print("len(queue[i]):",len(queue[i]))
        if len(queue[i]) > 0:
            queue_curr = i
            break
    #print("queue_curr:",queue_curr)
    if  queue_curr != -1:
        job_curr = queue[queue_curr][0]
        print(("time:",current_time," run job:",job_curr,"in queue:",queue_curr))
        current_time += 1
        jobctrl[job_curr]["timeleft"] -= 1
        jobctrl[job_curr]["time_slice_left"] -= 1
        jobctrl[job_curr]["alloc_time_left"] -= 1
        if  jobctrl[job_curr]["timeleft"] == 0:
            print(("time:",current_time,"end job:",job_curr))
            queue[queue_curr].pop(0)
            job_left -= 1
            continue
        if  jobctrl[job_curr]["time_slice_left"] > 0:
            continue
        else:
            if  jobctrl[job_curr]["alloc_time_left"] > 0:
                queue[queue_curr].pop(0)
                queue[queue_curr].append(job_curr)
                jobctrl[job_curr]["time_slice_left"] = time_slice
            else:
                queue[queue_curr].pop(0)
                if  queue_curr == 0:
                    queue[queue_curr].append(job_curr)
                else:
                    queue[queue_curr - 1].append(job_curr)
                jobctrl[job_curr]["time_slice_left"] = time_slice
                jobctrl[job_curr]["alloc_time_left"] = alloctime
    else:
        print("idle time")
        current_time += 1
print("end")


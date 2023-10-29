#! /usr/bin/env python 

from optparse import OptionParser
import random
import sys

seed = 1
def random_seed(seed):
    try:
        random.seed(seed, version=1)
    except:
        random.seed(seed)
    return

# process switch behavior
SCHED_SWITCH_ON_IO = 'SWITCH_ON_IO'
SCHED_SWITCH_ON_END = 'SWITCH_ON_END'

# io finished behavior
IO_RUN_LATER = 'IO_RUN_LATER'
IO_RUN_IMMEDIATE = 'IO_RUN_IMMEDIATE'

# process states
STATE_RUNNING = 'RUNNING'
STATE_READY = 'READY'
STATE_DONE = 'DONE'
STATE_WAIT = 'BLOCKED'

# members of process structure
PROC_CODE = 'code_'
PROC_ID = 'pid_'
PROC_STATE = 'proc_state_'
BLOCK_FINISH_TIME = 'block_finish_time_'

# things a process can do
DO_COMPUTE = 'cpu'
DO_IO = 'io'
DO_IO_DONE = 'io_done'

process_switch_behavior = SCHED_SWITCH_ON_IO
io_done_behavior = IO_RUN_IMMEDIATE


processes = {}

parser = OptionParser()
parser.add_option("-l", "--list", dest="process_list", help="input like this: -l m1:n1,m2:n2 ...  here m is nums of instruction  operation && n is probility of this instruction is cpu")
parser.add_option("-s", "--switch_action", action="store_true", dest="switch_action", default=False, help="default is SCHED_SWITCH_ON_IO if you don't use this flag")
parser.add_option("-i", "--io_done_next", action="store_true", dest="io_done_action", default=False, help="default is IO_RUN_IMMEDIATE if you don't use this flag")

(options, args) = parser.parse_args()

if options.switch_action:
    process_switch_behavior = SCHED_SWITCH_ON_END

if options.io_done_action:
    io_done_behavior = IO_RUN_LATER

for p in options.process_list.split(','):
    tmp = p.split(':')
    if len(tmp) != 2:
        print("wrong input and go to end !!!")
        exit(1)
    proc_id = len(processes)
    processes[proc_id] = {}
    # print('len(processes):',len(processes))
    # print('processes:',processes)
    processes[proc_id][PROC_ID] = proc_id
    processes[proc_id][PROC_STATE] = STATE_READY
    processes[proc_id][PROC_CODE] = []
    processes[proc_id][BLOCK_FINISH_TIME] = []
    num_instructions, chance_cpu = int(tmp[0]), float(tmp[1])/100.0
    for i in range(num_instructions):
        if random.random() < chance_cpu:
            processes[proc_id][PROC_CODE].append(DO_COMPUTE)
        else:
            processes[proc_id][PROC_CODE].append(DO_IO)
            processes[proc_id][PROC_CODE].append(DO_IO_DONE)

# print('processes[0][PROC_CODE]:',processes[0][PROC_CODE])
# print('processes[1][PROC_CODE]:',processes[1][PROC_CODE])
# print('processes[2][PROC_CODE]:',processes[2][PROC_CODE])
# print('processes[0][PROC_STATE]:',processes[0][PROC_STATE])
# print('processes[1][PROC_STATE]:',processes[1][PROC_STATE])
# print('processes[2][PROC_STATE]:',processes[2][PROC_STATE])
# print('processes[0][BLOCK_FINISH_TIME]:',processes[0][BLOCK_FINISH_TIME])
# print('processes[1][BLOCK_FINISH_TIME]:',processes[1][BLOCK_FINISH_TIME])
# print('processes[2][BLOCK_FINISH_TIME]:',processes[2][BLOCK_FINISH_TIME])

# print('process_switch_behavior:',process_switch_behavior);
# print('io_done_behavior:',io_done_behavior)

# exit(1)


# processes[0] = {}
# processes[0][PROC_STATE] = STATE_READY
# processes[0][PROC_ID] = 0
# processes[0][PROC_CODE] = [DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_IO, DO_IO_DONE, DO_COMPUTE]
# processes[0][BLOCK_FINISH_TIME] = []

# processes[1] = {}
# processes[1][PROC_STATE] = STATE_READY
# processes[1][PROC_ID] = 1
# processes[1][PROC_CODE] = [DO_IO, DO_IO_DONE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_IO, DO_IO_DONE, DO_COMPUTE]
# processes[1][BLOCK_FINISH_TIME] = []


# processes[2] = {}
# processes[2][PROC_STATE] = STATE_READY
# processes[2][PROC_ID] = 2
# processes[2][PROC_CODE] = [DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE]
# processes[2][BLOCK_FINISH_TIME] = []

# process_switch_behavior = SCHED_SWITCH_ON_IO
# io_done_behavior = IO_RUN_IMMEDIATE

time = -1

running_pid = -1

done_or_block = True;

io_nums = 0

print('%s' % 'Time', end='') 
for pid in range(len(processes)):
    print('%14s' % ('PID:%2d' % (pid)), end='')
print('%14s' % 'CPU', end='')
print('%14s' % 'IOs', end='')
print('')

while 1:
    time = time + 1
    # print('time:%d' % time)
    # print('processes[0][PROC_CODE]:',processes[0][PROC_CODE])
    # print('processes[1][PROC_CODE]:',processes[1][PROC_CODE])
    # print('processes[2][PROC_CODE]:',processes[2][PROC_CODE])
    # print('processes[0][PROC_STATE]:',processes[0][PROC_STATE])
    # print('processes[1][PROC_STATE]:',processes[1][PROC_STATE])
    # print('processes[2][PROC_STATE]:',processes[2][PROC_STATE])
    # print('processes[0][BLOCK_FINISH_TIME]:',processes[0][BLOCK_FINISH_TIME])
    # print('processes[1][BLOCK_FINISH_TIME]:',processes[1][BLOCK_FINISH_TIME])
    # print('processes[2][BLOCK_FINISH_TIME]:',processes[2][BLOCK_FINISH_TIME])
    # first part: process decision and  state change
    if running_pid == -1 and done_or_block:
        #
        for i in range(len(processes)):
            if processes[i][PROC_STATE] == STATE_READY:
                #
                processes[i][PROC_STATE] = STATE_RUNNING
                running_pid = i
                break
        if running_pid == -1:
            exist_block = False
            for i in range(len(processes)):
                if processes[i][PROC_STATE] == STATE_WAIT:
                    exist_block = True
                    break
            if not exist_block:
                break
    # print(' first part running_pid:%s' % running_pid)
    # Output 

    print('%2s' % (time), end='')
    for pid in range(len(processes)):
        if pid == running_pid:
            print('%14s' % ('RUN' + ':' + processes[pid][PROC_CODE][0]), end='')
        else:
            print('%14s' % (processes[pid][PROC_STATE]), end='')
    if running_pid != -1:
        print('%14s' % '1', end='')
    else:
        print('%14s' % ' ', end='')
    if io_nums == 0:
        print('%14s' % ' ', end='')
    else:
        print('%14s' % (io_nums), end='')
    print('')

    # second part: execute instruction

    execute_instruction = ''
    if running_pid != -1:
        #
        execute_instruction = processes[running_pid][PROC_CODE].pop(0)
    
    # thrid part: process change

    # if time == 10:
    #     print('time 10 running_pid:',running_pid)
    #     print('processes[0][PROC_CODE]:',processes[0][PROC_CODE])
    #     print('processes[1][PROC_CODE]:',processes[1][PROC_CODE])
    #     print('processes[2][PROC_CODE]:',processes[2][PROC_CODE])
    #     print('processes[0][PROC_STATE]:',processes[0][PROC_STATE])
    #     print('processes[1][PROC_STATE]:',processes[1][PROC_STATE])
    #     print('processes[2][PROC_STATE]:',processes[2][PROC_STATE])
    #     print('processes[0][BLOCK_FINISH_TIME]:',processes[0][BLOCK_FINISH_TIME])
    #     print('processes[1][BLOCK_FINISH_TIME]:',processes[1][BLOCK_FINISH_TIME])
    #     print('processes[2][BLOCK_FINISH_TIME]:',processes[2][BLOCK_FINISH_TIME])
    #     break

    if running_pid == -1:
        #
        for i in range(len(processes)):
            if len(processes[i][BLOCK_FINISH_TIME]) !=0 and time == processes[i][BLOCK_FINISH_TIME][len(processes[i][BLOCK_FINISH_TIME]) - 1]:
                processes[i][PROC_STATE] = STATE_READY
                io_nums -= 1
                if process_switch_behavior == SCHED_SWITCH_ON_END:
                    processes[i][PROC_STATE] = STATE_RUNNING
                    running_pid = i
                    break
                else:
                    if process_switch_behavior == SCHED_SWITCH_ON_IO and io_done_behavior == IO_RUN_IMMEDIATE:
                        processes[i][PROC_STATE] = STATE_RUNNING
                        running_pid = i
                        break
                    else:
                        #
                        break
    else:
        #
        for i in range(len(processes)):
            if len(processes[i][BLOCK_FINISH_TIME]) !=0 and time == processes[i][BLOCK_FINISH_TIME][len(processes[i][BLOCK_FINISH_TIME]) - 1]:
                processes[i][PROC_STATE] = STATE_READY
                io_nums -= 1

        if execute_instruction == DO_COMPUTE or execute_instruction == DO_IO_DONE:
            if len(processes[running_pid][PROC_CODE]) == 0:
                processes[running_pid][PROC_STATE] = STATE_DONE
                running_pid = -1
                if process_switch_behavior == SCHED_SWITCH_ON_END:
                    done_or_block = True
                #
                for i in range(len(processes)):
                    if len(processes[i][BLOCK_FINISH_TIME]) !=0 and time == processes[i][BLOCK_FINISH_TIME][len(processes[i][BLOCK_FINISH_TIME]) - 1]:
                        if process_switch_behavior == SCHED_SWITCH_ON_IO and io_done_behavior == IO_RUN_IMMEDIATE:
                            processes[i][PROC_STATE] = STATE_RUNNING
                            running_pid = i
                            break

        elif execute_instruction == DO_IO:
            processes[running_pid][PROC_STATE] = STATE_WAIT
            processes[running_pid][BLOCK_FINISH_TIME].append(time + 5)
            io_nums += 1
            running_pid = -1
            if process_switch_behavior == SCHED_SWITCH_ON_END:
                done_or_block = False
            if process_switch_behavior == SCHED_SWITCH_ON_IO :
                for i in range(len(processes)):
                    if len(processes[i][BLOCK_FINISH_TIME]) !=0 and time == processes[i][BLOCK_FINISH_TIME][len(processes[i][BLOCK_FINISH_TIME]) - 1]:
                        processes[i][PROC_STATE] = STATE_READY
                        if  io_done_behavior == IO_RUN_IMMEDIATE:
                            processes[i][PROC_STATE] = STATE_RUNNING
                            running_pid = i
                            break
                #print('running_pid:%s' % running_pid)
    #



print("END")







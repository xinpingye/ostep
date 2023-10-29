#! /usr/bin/env python 

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

processes = {}

processes[0] = {}
processes[0][PROC_STATE] = STATE_READY
processes[0][PROC_ID] = 0
processes[0][PROC_CODE] = [DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_IO, DO_IO_DONE, DO_COMPUTE]
processes[0][BLOCK_FINISH_TIME] = []

processes[1] = {}
processes[1][PROC_STATE] = STATE_READY
processes[1][PROC_ID] = 1
processes[1][PROC_CODE] = [DO_IO, DO_IO_DONE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_IO, DO_IO_DONE, DO_COMPUTE]
processes[1][BLOCK_FINISH_TIME] = []


processes[2] = {}
processes[2][PROC_STATE] = STATE_READY
processes[2][PROC_ID] = 2
processes[2][PROC_CODE] = [DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE, DO_COMPUTE]
processes[2][BLOCK_FINISH_TIME] = []

process_switch_behavior = SCHED_SWITCH_ON_IO
io_done_behavior = IO_RUN_IMMEDIATE

time = -1

running_pid = -1

done_or_block = True;

while 1:
    time = time + 1
    print('time:%d' % time)
    print('processes[0][PROC_CODE]:',processes[0][PROC_CODE])
    print('processes[1][PROC_CODE]:',processes[1][PROC_CODE])
    print('processes[2][PROC_CODE]:',processes[2][PROC_CODE])
    print('processes[0][PROC_STATE]:',processes[0][PROC_STATE])
    print('processes[1][PROC_STATE]:',processes[1][PROC_STATE])
    print('processes[2][PROC_STATE]:',processes[2][PROC_STATE])
    print('processes[0][BLOCK_FINISH_TIME]:',processes[0][BLOCK_FINISH_TIME])
    print('processes[1][BLOCK_FINISH_TIME]:',processes[1][BLOCK_FINISH_TIME])
    print('processes[2][BLOCK_FINISH_TIME]:',processes[2][BLOCK_FINISH_TIME])
    # first part: process decision
    if running_pid == -1 and done_or_block:
        #
        for i in range(len(processes)):
            if processes[i][PROC_STATE] == STATE_READY:
                #
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
    print(' first part running_pid:%s' % running_pid)
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
                print('running_pid:%s' % running_pid)

print("END")







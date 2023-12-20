#! /usr/bin/env python
import sys
class cpu:
    # some description for this class
    def __init__(self):
        self.PC = 0
        self.memory = {}
        for i in range(0, 1024 * 4):
            self.memory[i] = 0
        self.regs = {}
        for i in range(0, 6):
            self.regs[i] = 0
        self.conditions = {}
        for i in range(0,3):
            self.conditions[i] = -1
        self.register_name_to_nums_mapping = {"ax":0, "bx":1, "cx":2, "dx":3, "ex":4, "fx":5}
        self.var = {}
    # functions for specila instruction
    def mov_i_to_m(self, src, dst):
        self.memory[dst] = src
        return 0
    def mov_r_to_m(self, src, dst):
        self.memory[dst] = self.regs[src]
        return 0
    def mov_m_to_r(self, src, dst):
        self.regs[dst] = self.memory[src]
        return 0
    def mov_m_to_r_2(self, memory_addr, reg1_value, reg2_value, mult, dst):
        src = memory_addr + self.regs[reg1_value] + self.regs[reg2_value] * mult
        print("src:", src)
        self.regs[dst] = self.memory[src]
        return 0;
    def mov_r_to_m_2(self, src, memory_addr, reg1_value, reg2_value, mult):
        dst = memory_addr + self.regs[reg1_value] + self.regs[reg2_value] * mult
        print("dst:", dst)
        self.memory[dst] = self.regs[src]
        return 0
    def add_i_to_r(self, i, r):
        self.regs[r] += i
        return 0
    def sub_i_from_r(self, i, r):
        self.regs[r] -= i
        return 0
    def xchg_r_and_m(self, r, m):
        tmp = self.memory[m]
        self.memory[m] = self.regs[r]
        self.regs[r] = tmp
        return 0
    def test_i_and_r(self, i, r):
        self.conditions = {0:0, 1:0, 2:0, 3:0}
        reg_val = self.regs[r]
        if (reg_val > i):
            self.conditions[0] = 1
        if (reg_val < i):
            self.conditions[1] = 1
        if (reg_val == i):
            self.conditions[2] = 1
        if (reg_val != i):
            self.conditions[3] = 1
        return 0
    def jump_greaterthan(self, target):
        if (self.conditions[0] == 1):
            self.PC = target
        return 0
    def jump_lessthan(self, target):
        if (self.conditions[1] == 1):
            self.PC = target
        return 0
    def jump_equal(self, target):
        if (self.conditions[2] == 1):
            self.PC = target
        return 0
    def jump_not_equal(self, target):
        if (self.conditions[3] == 1):
            self.PC = target
        return 0
    def halt(self):
        return -1
    def from_data_str_to_data(self, str):
        if (str[0] == '$'):
            str_immediate_nums = str[1:]
            return 0,int(str_immediate_nums)
        elif(str[0] == '%'):
            register_num = self.register_name_to_nums_mapping[str[1:]]
            return 1, register_num
        elif(str[0].isdigit()):
            if (str.isdigit()):
               return 2, int(str)
            else:
                memory_addr = int(str.split('(')[0])
                tmp = str.split('(')[1].split(')')[0]
                tmp_list = tmp.split(':')
                if  len(tmp_list) == 1:
                    reg_value = self.regs[self.register_name_to_nums_mapping[str.split('(')[1].split(')')[0][1:]]]
                    return 2, memory_addr + reg_value
                elif len(tmp_list) == 3:
                    reg1 = self.register_name_to_nums_mapping[tmp_list[0][1:]]
                    reg2 = self.register_name_to_nums_mapping[tmp_list[1][1:]]
                    # reg1_value = self.regs[self.register_name_to_nums_mapping[reg1]]
                    # reg2_value = self.regs[self.register_name_to_nums_mapping[reg2]]
                    mult = int(tmp_list[2])
                    # print('reg1:', reg1, 'reg2:', reg2, 'reg1_value:', reg1_value, 'reg2_value:', reg2_value, 'mult:', mult)
                    return 3, [memory_addr, reg1 , reg2 , mult]
        elif str in self.var:
            return 2, self.var[str]          
    def load(self, file, loadaddr):
        fd = open(file)
        pc = loadaddr
        name_to_pc_mapping = {}
        var_addr = 100
        for line in fd:
            if len(line.strip()) == 0:
                continue
            line_split = line.split('#')
            if (len(line_split[0]) == 0):
                continue
            if line[0] == '.':
                line = line.strip()
                line_split_list = line.split()
                if len(line_split_list) == 1:
                    name_to_pc_mapping[line] = pc
                    continue 
                else:
                    if  line_split_list[0] == '.var':
                        self.var[line_split_list[1]] = var_addr
                        mult = 1
                        if  len(line_split_list) == 3:
                            mult = int(line_split_list[2])
                        var_addr += mult * 4
                        continue
            one_command = line_split[0].strip()
            opcode = one_command.split(None, 1)[0]
            if (opcode == 'mov' or opcode == 'add' or opcode == 'sub' or opcode\
            == 'test' or opcode == 'jgt' or opcode == 'jgte' or opcode == 'je'\
            or opcode == 'jne' or opcode == 'halt' or opcode == 'xchg'):
                pc += 1
        fd.close()
        fd = open(file)
        pc = loadaddr
        for line in fd:
            if len(line.strip()) == 0:
                continue
            line_split = line.split('#')
            if (len(line_split[0]) == 0):
                continue
            if line[0] == '.':
                line = line.strip()
                name_to_pc_mapping[line] = pc
                continue
            one_command = line_split[0].strip()
            opcode = one_command.split(None, 1)[0]
            if (opcode == 'mov'):
                datas = one_command.split(None, 1)[1]
                first_data = datas.split(',')[0]
                second_data = datas.split(',')[1].strip()
                first_type, first_data = self.from_data_str_to_data(first_data)
                second_type, second_data = self.from_data_str_to_data(second_data)
                if (first_type == 0 and second_type == 2):
                    self.memory[pc] = 'self.mov_i_to_m(%d, %d)' % (first_data, second_data)
                    pc += 1
                elif(first_type == 1 and second_type == 2):
                    self.memory[pc] = 'self.mov_r_to_m(%d, %d)' % (first_data, second_data)
                    pc += 1   
                elif(first_type == 2 and second_type == 1):
                    self.memory[pc] = 'self.mov_m_to_r(%d, %d)' % (first_data, second_data)
                    pc += 1
                elif(first_type == 3 and second_type == 1):
                    self.memory[pc] = 'self.mov_m_to_r_2(%d, %d, %d, %d ,%d)' % (\
                                                         first_data[0], first_data[1],\
                                                         first_data[2], first_data[3],\
                                                         second_data)
                    pc += 1
                elif(first_type == 1 and second_type == 3):
                    self.memory[pc] = 'self.mov_r_to_m_2(%d, %d, %d, %d ,%d)' % (first_data, \
                                                         second_data[0], second_data[1],\
                                                         second_data[2], second_data[3])
                    pc += 1
            elif (opcode == 'add'):
                datas = one_command.split(None, 1)[1]
                first_data = datas.split(',')[0]
                second_data = datas.split(',')[1].strip()
                first_type, first_data = self.from_data_str_to_data(first_data)
                second_type, second_data = self.from_data_str_to_data(second_data)
                self.memory[pc] = 'self.add_i_to_r(%d, %d)' % (first_data, second_data)
                pc += 1
            elif (opcode == 'sub'):
                datas = one_command.split(None, 1)[1]
                first_data = datas.split(',')[0]
                second_data = datas.split(',')[1].strip()
                first_type, first_data = self.from_data_str_to_data(first_data)
                second_type, second_data = self.from_data_str_to_data(second_data)
                self.memory[pc] = 'self.sub_i_from_r(%d, %d)' % (first_data, second_data)
                pc += 1
            elif (opcode == 'xchg'):
                datas = one_command.split(None, 1)[1]
                first_data = datas.split(',')[0]
                second_data = datas.split(',')[1].strip()
                first_type, first_data = self.from_data_str_to_data(first_data)
                second_type, second_data = self.from_data_str_to_data(second_data)
                self.memory[pc] = 'self.xchg_r_and_m(%d, %d)' % (first_data, second_data)
                pc += 1
            elif (opcode == 'test'):
                datas = one_command.split(None, 1)[1]
                first_data = datas.split(',')[0]
                second_data = datas.split(',')[1].strip()
                first_type, first_data = self.from_data_str_to_data(first_data)
                second_type, second_data = self.from_data_str_to_data(second_data)
                self.memory[pc] = 'self.test_i_and_r(%d, %d)' % (first_data, second_data)
                pc += 1
            elif (opcode == 'jgt' or opcode == 'jgte'):
                target = one_command.split(None, 1)[1]
                target_pc = name_to_pc_mapping[target]
                self.memory[pc] = 'self.jump_greaterthan(%d)' % (target_pc)
                pc += 1
            elif (opcode == 'je'):
                target = one_command.split(None, 1)[1]
                target_pc = name_to_pc_mapping[target]
                self.memory[pc] = 'self.jump_equal(%d)' % (target_pc)
                pc += 1
            elif (opcode == 'jne'):
                target = one_command.split(None, 1)[1]
                target_pc = name_to_pc_mapping[target]
                self.memory[pc] = 'self.jump_not_equal(%d)' % (target_pc)
                pc += 1
            elif (opcode == 'halt'):
                self.memory[pc] = 'self.halt()'
                pc += 1
    # def run(self, one_process):
    #     one_process.restore()
    #     self.PC = one_process.pc
    #     pc_prev = self.PC
    #     i = 10
    #     while True:            
    #         print(self.memory[self.PC])
    #         rc = eval(self.memory[self.PC])
    #         if pc_prev == self.PC:
    #             self.PC += 1
    #         pc_prev = self.PC
    #         if rc == -1:
    #             return
    #         i -= 1
    def run(self, processes, interval):
        processes.p_list[processes.cur].restore()
        print(processes.p_list[processes.cur].tid)
        pc_prev = self.PC
        i = 15
        time = 0
        # while i != 0:
        while True:
            # i -= 1
            # print('interval:', interval, 'time:', time)
            print(self.memory[self.PC])
            rc = eval(self.memory[self.PC])
            if pc_prev == self.PC:
                self.PC += 1
            pc_prev = self.PC
            time += 1
            if rc == -1:
                if  processes.one_process_done() == -1:
                    return
                processes.p_list[processes.cur].restore()
                print(processes.p_list[processes.cur].tid)
                pc_prev = self.PC
                if time == interval:
                    time = 0
                continue 
            if time == interval:
                processes.p_list[processes.cur].save()
                processes.next_run_process()
                processes.p_list[processes.cur].restore()
                print(processes.p_list[processes.cur].tid)
                pc_prev = self.PC
                time = 0


        
class process:
    def __init__(self, cpu, tid, pc, regs={0:0, 1:0, 2:0, 3:0, 4:0, 5:0}):
        self.cpu = cpu
        self.tid = tid
        self.pc = pc
        self.regs = regs
        self.conditions = {0:0, 1:0, 2:0, 3:0}
        self.done = False
    def get_pc(self):
        return self.pc
    def get_tid(self):
        return self.tid
    def set_done(self):
        self.done = True
    def is_done(self):
        return self.done == True
    def save(self):
        self.regs = self.cpu.regs
        self.pc = self.cpu.PC
        self.conditions = self.cpu.conditions    
    def restore(self):
        self.cpu.regs = self.regs
        self.cpu.PC = self.pc
        self.cpu.conditions = self.conditions
class process_list:
    def __init__(self):
        self.p_list = []
        self.cur = 0
        self.active = 0
    def add(self, p):
        self.p_list.append(p)
        self.active += 1
    def one_process_done(self):
        self.active -= 1
        self.p_list[self.cur].set_done()
        if self.active == 0:
            return -1
        self.next_run_process()
        return 0
    def next_run_process(self):
        for i in range(self.cur + 1, len(self.p_list)):
            if self.p_list[i].is_done():
                continue
            self.cur = i
            return
        for i in range(0, self.cur + 1):
            if self.p_list[i].is_done():
                continue
            self.cur = i
            return


cpu = cpu()
file = sys.argv[1]
process_nums = int(sys.argv[2])
interval = int(sys.argv[3])
cpu.load(file,1000)
processes = process_list()
for i in range(0, process_nums):
    process_tmp = process(cpu, i, 1000, {0:1, 1:1, 2:1, 3:3, 4:0, 5:0})
    processes.add(process_tmp)
cpu.run(processes, interval)
# cpu.run(process(cpu, 1, 1000, {0:1, 1:0, 2:0, 3:4}))
print('count:', cpu.memory[cpu.var['count']], 'ax:', cpu.regs[0], 'bx:', cpu.regs[1], \
      'cx:', cpu.regs[2], 'dx:', cpu.regs[3], 'ex:', cpu.regs[4], 'fx:', cpu.regs[5])
print("end")
    
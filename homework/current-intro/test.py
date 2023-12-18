#! /usr/bin/env python

# import hexdump
from codecs import encode

def dospace(howmuch):
    for i in range(howmuch):
        print('%24s' % ' ', end=' ')

print('%10s' % '12345')
print('%20s' % '12345')

def zassert(cond, str):
    if cond == False:
        print('ABORT::', str)
        exit(1)
    return
map_test = {"abc":1, "bca":2, "cab":3}

if "abcd" not in map_test:
    print("map && not in test success")
else:
    print("map && in test fail")
# zassert(0,"test zassert")

memory = {}

for i in range(5):
    memory[i] = 0
print(memory)

str = "I am app 1"
print(str.replace("am","XXX"))

arg = "$200"
tmp1 = arg.split('$')[1]
print(tmp1.isdigit())
arg = "(%ax)"
print(arg.split('('))

print("%d,%d,%d" % (0,1,2))

arg = '.text'
tmp = arg.replace(',','')
print(tmp)

file = open("1.txt")

for line in file:
    print('line:%s' % (line))

one_comment = "    # this is a comment!!!"
print(one_comment.split('#'))

specila_str_for_split_no_arguement = "   12334  \t 1233\n!!!"
print(specila_str_for_split_no_arguement.split())

str_for_rstrip = "   1233 234   \n"
print(str_for_rstrip)
print("test")
print(str_for_rstrip.rstrip())
print("test")
str = ".main"
print(str.split())

str = ".main\n.top\nsub  $1,%dx\ntest $0,%dx     \njgte .top         \nhal\
t"
print(str)

strstr = "  123 2345 4567\n"
print(strstr.split(None, 3))
strstr = " 123"
print(strstr.split(' '))

str = "111   1234567     1345     111"
print(str.strip('1'))

instruction = "   sub  $1,%dx # some comment"
print(instruction)
instruction = instruction.split('#')[0]
print(instruction)
instruction = instruction.strip()
print(instruction)
instruction = instruction.split(None, 1)
print(instruction)
opcode = instruction[0]
arg = instruction[1]
print('opcode:%s' % (opcode))
print('arg:%s' % (arg))
args = arg.split(',', 1)
print('args:%s' % (args))
arg1 = args[0]
arg2 = args[1]
print('arg1:%s' % (arg1))
print('arg2:%s' % (arg2))

# str_test = "123    23456"
# print(str_test.split(' '))
# print(str_test.split(None, 1))

# memory = {}
# memory['abc'] = 1
# memory['bca'] = "123456"
# print(memory)

# str = '123456:%d  abc:%d tuu:%s' % (1,2,'3')
# print(str)

def func(arg1, arg2, arg3):
    nums = arg1 + arg2
    print(nums,arg3)
    return 0

str = 'func(%d, %d, %s)' % (1, 2, "3")
print(str)
eval(str)
str = '#123456 566'
print(str.split('#'))
str = '   123456 2345  '
print(str)
for character in str:
    print(character, character.encode('utf-8').hex())
# hexdump.dump(str, sep = ':')
str = str.strip()
print("***************")
# hexdump.dump(str, sep = ':')
for character in str:
    print(character, character.encode('utf-8').hex())

instruction = "123  345   1234566"
print(instruction.split(' ', 2))
print(instruction.split(None, 2))

data = '$12345'
print(data[1:])

if (1 == 2):
    print(1)
elif(2 == 2):
    print(2)

str = "11111"
print("222",str,"333")
str = "     \n"
str = str.strip()
print(len(str))
str = str.split(None, 1)
print(str)
print(len(str))
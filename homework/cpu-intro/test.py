#! /usr/bin/env python 

print("123","234","345")

str = "mystring"

num = 123456

print('this is str: %s and this is num: %d  next str: %s ' % (str,num,'string for another'))

def space(num_columns):
    for i in range(num_columns):
        print('%10s' % ' ', end='')
print('%s' % 'Time')
for pid in range(5):
    print('%14s' % ('PID:%2d' % (pid)), end='')
print();
print('%3d*' % 1)

from optparse import OptionParser

# 创建 OptionParser 实例
parser = OptionParser()

# 添加命令行选项
parser.add_option("-f", "--file", dest="filename", help="input filename")
parser.add_option("-v", action="store_true", dest="verbose", default=False, help="enable verbose mode")

# 解析命令行参数
(options, args) = parser.parse_args()

# 访问解析后的参数
if options.verbose:
    print("Verbose mode is enabled.")

if options.filename:
    print("Input filename:", options.filename)

# 处理额外的参数
if args:
    print("Extra arguments:", args)


#space(5)
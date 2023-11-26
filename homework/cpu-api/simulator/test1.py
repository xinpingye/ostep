#! /usr/bin/env python3
import string
import time

str  = string.ascii_lowercase + string.ascii_uppercase
result = ""

for i in range(len(str) - 1):
    result += (str[i] + "+" + str[i+1])
    if i != len(str) -1 -1:
        result += ","

a = [12]
b = [23]
c = [34]

merge = a + b

print(merge)

a += b

action = "a+b"
tmp = action.split('+')
print(tmp)
action = "a-"
tmp = action.split('-')
print(tmp)

my_string = "Hello, World!"
result = my_string[-1:]
print(result)

my_string2 = ['123456']
result = my_string2[:-1]
print(result)
print("range(0):",(range(4)))


print(a+b+c)
print(result)
print((u'\u2502', u'\u2500', u'\u251c', u'\u2514'))
print(('\u2502', '\u2500', '\u251c', '\u2514'))
print(string.ascii_lowercase + string.ascii_uppercase)

chars = (u'\u2502', u'\u2500', u'\u251c', u'\u2514')
print('a')
print('%s%s%s ' % (chars[3], chars[1], chars[1]), end='')
print('b')

print("xxx")
print("%s%s%s%s"% (chars[3], chars[1], chars[1], chars[1]),end=' ')
print("xxx")

print("xxx")
print('%s%s%s ' % (chars[2], chars[1], chars[1]), end='')
print("xxx")
print('%s%s%s ' % (chars[3], chars[1], chars[1]), end='')
print("xxx")


children = {}
children['a'] = ['b', 'c']
children['b'] = ['d', 'e']
children['d'] = ['f']
children['c'] = ['g']
children['e'] = []
children['f'] = []
children['g'] = []

def walk(p, level, pmask, is_last):
    print('                               ', end='')
    chars = (u'\u2502', u'\u2500', u'\u251c', u'\u2514')
        
    # print stuff before node
    if level > 0:
        # main printing
        for i in range(level-1):
            if pmask[i]:
                # '|  '
                time.sleep(3)
                print('%s   ' % chars[0], end='')
            else:
                time.sleep(3)
                print('    ', end='')
        if pmask[level-1]:
            # '|__'
            if is_last:
                time.sleep(3)
                print('%s%s%s ' % (chars[3], chars[1], chars[1]), end='')
            else:
                time.sleep(3)
                print('%s%s%s ' % (chars[2], chars[1], chars[1]), end='')
        else:
            # '___' 
            time.sleep(3)
            print(' %s%s%s ' % (chars[1], chars[1], chars[1]), end='')

    # print node
    time.sleep(3)
    print('%s' % p)

    # undo parent verticals
    if is_last:
        pmask[level-1] = False

    # recurse
    pmask[level] = True
    for child in children[p][:-1]:
        walk(child, level + 1, pmask, False)
    for child in children[p][-1:]:
        walk(child, level + 1, pmask, True)
    return

def print_tree():
    return walk('a', 0, {}, False)
print_tree()



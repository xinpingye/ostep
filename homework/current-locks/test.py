#! /usr/bin/env python

str = '1234  3456  2345    1345'
print(str.split())
str = 'xxx)'
print(str.split(')'))
str = '(%fx,%cx,4)'
print(str.split('(')[1].split(')')[0].split(','))

def test_func():
    return 1,2,3,4,5
rc1,rc2 = test_func()
print(rc1, rc2)

nums = 2023

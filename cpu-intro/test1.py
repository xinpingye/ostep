#! /usr/bin/env python
import sys
from optparse import OptionParser

usage=" this %prog I am usage"
parser = OptionParser(usage=usage)

parser.add_option("-f", "--file", action="store", type="string", dest="filename", help="help test1  [%default]",metavar="FILE1",default="default filename")
parser.add_option("-n", "--num", type="int", dest="num", help="help test2",metavar="number")
parser.add_option("-v", action="store_true", dest="verbose", help="help test3")
parser.add_option("-q", action="store_false", dest="verbose", help="test4")

args = ["-f", "foo.txt", "-n", "123", "other args","-v", "-q"]
(options, args) = parser.parse_args()

print(args)

print(options.filename)
print(options.num)
print(options.verbose)

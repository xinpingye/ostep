#! /usr/bin/env python
import sys
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-f", "--file", action="store", type="string", dest="filename")
parser.add_option("-n", type="int", dest="num")
parser.add_option("-v", action="store_true", dest="verbose")
parser.add_option("-q", action="store_false", dest="verbose")

args = ["-f", "foo.txt", "-n", "123", "other args","-v", "-q"]
(options, args) = parser.parse_args(args)

print(args)

print(options.filename)
print(options.num)
print(options.verbose)

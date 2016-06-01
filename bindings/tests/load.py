#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIodefEasy
except Exception,e:
	print "Import failed: ",e
	print "Try 'cd ./.libs && ln -s libiodef_python.so _LibIodefEasy.so'"
	sys.exit(1)

iodef = LibIodefEasy.IODEF()
iodef.ReadFromFile("foo.bin")
iodef.PrintToStdout()

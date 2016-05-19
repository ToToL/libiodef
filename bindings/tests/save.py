#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import PreludeEasy
except Exception,e:
	print "Import failed: ",e
	print "Try 'cd ./.libs && ln -s libprelude_python.so _PreludeEasy.so'"
	sys.exit(1)

iodef = PreludeEasy.IODEF()
iodef.Set("alert.classification.text", "Bar")
iodef.WriteToFile("foo.bin")

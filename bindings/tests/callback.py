#!/usr/bin/python

import sys
sys.path.append('.')
sys.path.append('./.libs')

try:
	import LibIodefEasy
except:
	print "Import failed"
	print "Try 'cd ./.libs && ln -s libiodef_python.so _LibIodefEasy.so'"
	sys.exit(1)

def foo(id):
        print "callback: id = " + str(id)
	iodef = LibIodefEasy._get_IODEF(id)
        iodef.PrintToStdout()
        #print bar.Get("alert.classification.text") # XXX not yet implemented
        return 0

LibIodefEasy.set_pymethod(foo)

LibIodefEasy.test_fct()

#!/usr/bin/python

import os
import sys
import PreludeEasy


##################
# this program will save all alerts to the "alerts" directory.
#
# known bu^Wfeatures: directory must exist, and old alerts are overwritten if present
##################

i = 0
dest_dir = "alerts"
if len(sys.argv) > 1:
	dest_dir = sys.argv[1]


if os.path.exists(dest_dir) == 0:
	print "dir ",dest_dir," does not exist"
	sys.exit(1)


client = PreludeEasy.ClientEasy("PolluxTest", PreludeEasy.Client.IODEF_READ)
client.SetFlags(client.GetFlags() & ~PreludeEasy.Client.HEARTBEAT)
#client.Init()
client.Start()

#ret = client.PoolInit("192.168.33.215", 1)
#print "PoolInit =>",ret


def handle_alert(iodef):
	global i
	try:
		print iodef
		i += 1
		f = open( "%s/%d.iodef" % (dest_dir,i), "w" )
		iodef >> f
		f.close()
		print i
	except RuntimeError,_e:
		print "An exception occured: ",_e


import time

while 1:
	sys.stdout.write(".")
	sys.stdout.flush()
	iodef = PreludeEasy.IODEF()
	client >> iodef
	if iodef:
		handle_alert(iodef)
	time.sleep(2)

#!/usr/bin/python

import LibIodefEasy

client = LibIodefEasy.ClientEasy("PoolingTest", LibIodefEasy.Client.IODEF_READ)
client.Start()

while True:
    iodef = LibIodefEasy.IODEF()

    ret = client.RecvIODEF(iodef)
    if ret:
	print iodef

#!/usr/bin/python

import PreludeEasy

client = PreludeEasy.ClientEasy("PoolingTest", PreludeEasy.Client.IODEF_READ)
client.Start()

while True:
    iodef = PreludeEasy.IODEF()

    ret = client.RecvIODEF(iodef)
    if ret:
	print iodef

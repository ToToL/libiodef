#!/usr/bin/python

import sys
import LibIodefEasy

iodef = LibIodefEasy.IODEF()
iodef.Set("alert.classification.text", "Bar")

client = LibIodefEasy.ClientEasy("MyTest")
client << iodef


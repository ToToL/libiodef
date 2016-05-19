#!/usr/bin/python

import sys
import PreludeEasy

iodef = PreludeEasy.IODEF()
iodef.Set("alert.classification.text", "Bar")

client = PreludeEasy.ClientEasy("MyTest")
client << iodef


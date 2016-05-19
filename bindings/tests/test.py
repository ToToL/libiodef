#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import iodef

iodef = iodef.IODEF()

print("*** IODEF->Set() ***")
iodef.set("alert.classification.text", "My Message")
iodef.set("alert.source(0).node.address(0).address", "s0a0")
iodef.set("alert.source(0).node.address(1).address", "s0a1")
iodef.set("alert.source(1).node.address(0).address", "s1a0")
iodef.set("alert.source(1).node.address(1).address", "s1a1")
iodef.set("alert.source(1).node.address(2).address", None)
iodef.set("alert.source(1).node.address(3).address", "s1a3")
print(iodef)

print("\n*** Value IODEF->Get() ***")
print(iodef.get("alert.classification.text"))

print ("\n*** Listed Value IODEF->Get() ***")
print (iodef.get("alert.source(*).node.address(*).address"))

print ("\n*** Object IODEF->Get() ***")
print (iodef.get("alert.source(0).node.address(0)"))

print ("\n*** Listed Object IODEF->Get() ***")
print (iodef.get("alert.source(*).node.address(*)"))



#!/usr/bin/env ruby 

require("Prelude")

Prelude::PreludeLog::setCallback(lambda{|level,str|print "log: " + str})
iodef = Prelude::IODEF.new()

print "*** IODEF->Set() ***\n"
iodef.set("alert.classification.text", "My Message")
iodef.set("alert.source(0).node.address(0).address", "s0a1")
iodef.set("alert.source(0).node.address(1).address", "s0a2")
iodef.set("alert.source(1).node.address(0).address", "s1a1")
iodef.set("alert.source(1).node.address(1).address", "s1a2")
iodef.set("alert.source(1).node.address(2).address", nil)
iodef.set("alert.source(1).node.address(3).address", "s1a3")
print iodef

print "\n*** Value IODEF->Get() ***\n"
print iodef.get("alert.classification.text")

print "\n\n*** Listed Value IODEF->Get() ***\n"
print iodef.get("alert.source(*).node.address(*).address")

print "\n\n*** Object IODEF->Get() ***\n"
print iodef.get("alert.source(0).node.address(0)")

print "\n\n*** Listed Object IODEF->Get() ***\n"
print iodef.get("alert.source(*).node.address(*)")
print "\n\n"

fd = File.new("foo.bin", "w")
iodef >> fd
#iodef.Write(fd)
fd.close()

print "\n*** IODEF->Read() ***\n"
fd2 = File.new("foo.bin", "r")
iodef2 = Prelude::IODEF.new()
while true do
	begin
		iodef2 << fd2
		print iodef2
	rescue EOFError
                print "Got EOF\n"
		break
	end
end
fd2.close()

fd2 = File.new("foo.bin", "r")
iodef2 = Prelude::IODEF.new()
while iodef2.read(fd2) > 0 do
	print iodef2
end
fd2.close()

print "\n*** Client ***\n"
c = Prelude::ClientEasy.new("prelude-lml")
c.start()

c << iodef

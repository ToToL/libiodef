#!/usr/bin/env lua

require("libiodef")

function my_cb(level, log)
	io.write("log: " .. log)
end
libiodef.LibIodefLog_setCallback(my_cb)


function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif not v then
      print(formatting .. 'NUL')
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))
    else
      print(formatting .. tostring(v))
    end
  end
end


iodef = libiodef.IODEF()

print("*** IODEF->Set() ***")
iodef:set("alert.classification.text", "My Message")
iodef:set("alert.source(0).node.address(0).address", "s0a0")
iodef:set("alert.source(0).node.address(1).address", "s0a1")
iodef:set("alert.source(1).node.address(0).address", "s1a0")
iodef:set("alert.source(1).node.address(1).address", "s1a1")
iodef:set("alert.source(1).node.address(2).address", nil)
iodef:set("alert.source(1).node.address(3).address", "s1a3")
print(iodef)

print("\n*** Value IODEF->Get() ***")
print(iodef:get("alert.classification.text"))

print("\n*** Listed Value IODEF->Get() ***")
tprint(iodef:get("alert.source(*).node.address(*).address"))

print("\n*** Object IODEF->Get() ***")
print(iodef:get("alert.source(0).node.address(0)"))

print("\n*** Listed Object IODEF->Get() ***")
tprint(iodef:get("alert.source(*).node.address(*)"))


fd = io.open("foo.bin","w")
iodef:write(fd)
fd:close()

print("\n*** IODEF->Read() ***")
fd2 = io.open("foo.bin","r")
iodef2 = libiodef.IODEF()
while iodef2:read(fd2) do
	print(iodef2)
end
fd2:close()


print("\n*** Client ***")
c = libiodef.ClientEasy("libiodef-lml")
c:start()

c:sendIODEF(iodef)

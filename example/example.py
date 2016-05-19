import iodef
import datetime

msg = iodef.IODEF()


msg.set("incident(0).incident_id.name", "a")
msg.set("formatid", "formatid")
msg.set("version", "1.0")

msg.set("incident(0).event_data(0).record.record_data(0).record_item(0).data", "Record Item")
msg.set("incident(0).event_data(0).record.record_data(0).record_item(0).dtype", "string")
msg.set("incident(0).report_time", str(datetime.datetime.now()))

print(msg)
print(msg.toJSON())

msg2 = iodef.IODEF(msg.toJSON())
print(msg2)


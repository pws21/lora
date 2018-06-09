lora = require('test')

conn = lora.connect("onm", "onm", "onyma11")
result = conn:execute("select user from dual where :x = :y", {x=1,y=2})

for rs in conn:iterate("select * from ac_user") do
    print( table.concat(rs,",") )
end

conn:commit()
conn:close()
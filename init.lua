package.cpath = "./?.so"

require("test")

--print(cube(3))
--print(dpi())

--conn = ora_conn("onm", "onm", "onyma11")

ConnectionClass = {
    __index = {
        execute = function(self, sql)
            print("Executing "..sql)
        end,
        commit = function(self)
        end,
        rollback = function(self)
        end,
        close,
        ping,
        
    }
}


function connect(username, password, dbname)
    local oraconn = ora_conn(username, password, dbname)
end
-- Objects
Account = {balance=0}

function Account.add(v)
    Account.balance = Account.balance + v
end

print(Account.balance)
Account.add(100)
print(Account.balance)

-- Object with self
Account = {balance=0}

function Account.add(self, v)
    self.balance = self.balance + v
end

al = Account; Account = nil
al.add(al, 100)
print(al.balance)

al:add(20)
print(al.balance)

ConnectionClass = {
    __index = {
        execute = function(self, sql)
            print("Executing "..sql)
        end
    }
}
conn = setmetatable({}, ConnectionClass)

ConnectionClass = {}
function ConnectionClass:execute(self, sql)
    print("Executing "..sql)
end

conn = ConnectionClass
conn:execute("select user")
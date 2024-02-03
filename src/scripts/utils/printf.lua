require("utils.string")

function printf(fmt, ...)
    local args = {...}
    local text = fmt:gsub("%b{}", function()
        return tostring(table.remove(args, 1))
    end)

    print(text)
    return text
end
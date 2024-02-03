require("utils.printf")

events = {}
local event_handlers = {}

function events.register(event, handler)
    if not event_handlers[event] then
        event_handlers[event] = {}
    end

    printf("register event {} | {}", event, handler)
    table.insert(event_handlers[event], handler)
end

function events.dispatch(event, ...)
    local payload = {...}
    local handlers = event_handlers[event]
    if handlers then
        printf("dispatching event {} to {} listeners", event, #handlers)
        for _, handler in ipairs(handlers) do
            handler(payload)
        end
    end
end

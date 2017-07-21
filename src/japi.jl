include("j_utils.jl")

function garapa_hello()
    println("Garapa Julia API Loaded")
    garapa_register_callback_cpu_loop(dummy)
    garapa_register_callback_display_update(dummy)
end

callback_display_update = _ -> 0
callback_cpu_loop       = _ -> 0

function garapa_cpu_loop()
    callback_cpu_loop()
end

function garapa_display_update()
    callback_display_update()
end

function garapa_register_callback_display_update(func)
    println("Updated display_update callback: ", func)
    global callback_display_update = func
end

function garapa_register_callback_cpu_loop(func)
    global callback_cpu_loop = func
end

function dummy()

end


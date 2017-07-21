function garapa_hello()
    println("Garapa Julia API Loaded")
end

function garapa_read_pc()
    return ccall(:garapa_jl_read_pc, UInt16, ())
end

function garapa_read_byte( addr :: UInt16 )
    return ccall(:garapa_jl_read_byte, UInt8, (UInt16,), addr)
end

function garapa_cpu_loop()
    if garapa_read_byte(0xffe1) == 0x07
        #=println("Yay, menu")=#
    end
end

function garapa_display_update()
    #=@printf("Pc is %04x\n", garapa_read_pc())=#
end


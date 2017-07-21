function garapa_read_pc() :: Int64
    return ccall(:garapa_jl_read_pc, UInt16, ())
end

function garapa_write_byte( addr :: UInt16, data :: UInt8 ) :: Void
    ccall(:garapa_jl_write_byte, UInt8, (UInt16, UInt8), addr, data)
end

function garapa_read_byte( addr :: UInt16 ) :: Int64
    return ccall(:garapa_jl_read_byte, UInt8, (UInt16,), addr)
end


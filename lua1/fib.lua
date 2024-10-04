function fibonacci(n)
    if n <= 1 then
        return n
    end
    return fibonacci(n - 1) + fibonacci(n - 2)
end

local result = fibonacci(40)
print("40 Fibonacci: " .. result)

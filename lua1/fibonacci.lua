#!/bin/env lua

function fibonacci_recursive(n)
    if n <= 0 then
        return 0
    elseif n == 1 then
        return 1
    else
        return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2)
    end
end

-- 动态规划实现 Fibonacci 数列
function fibonacci_dynamic(n)
    if n <= 0 then
        return 0
    elseif n == 1 then
        return 1
    end

    local fib = {}
    fib[0] = 0
    fib[1] = 1

    for i = 2, n do
        fib[i] = fib[i - 1] + fib[i - 2]
    end

    return fib[n]
end

local n = 40
print("Fibonacci (recursive) of " .. n .. ": " .. fibonacci_recursive(n))
-- print("Fibonacci (dynamic) of " .. n .. ": " .. fibonacci_dynamic(n))


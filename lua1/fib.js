function fibonacci(n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

const result = fibonacci(40);
console.log(`40 Fibonacci: ${result}`);

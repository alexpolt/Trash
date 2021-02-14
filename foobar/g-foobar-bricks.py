import math

def solution(n):
    # Your code here
    
    def calc_lower_bound(n):
        return math.ceil(-0.5 + 0.5*math.sqrt(1+8*n))

    seen = {1: 0, 2: 0}
    stack = []
    counter = carry = 0
    limit = n

    while True:
        if (n, carry) in seen:
            count = seen[(n, carry)]
            n, carry, limit, counter = stack.pop()
            counter += count

        carry += 1
        min_n = calc_lower_bound(n)

        if n - carry < min_n:
            if not stack:
                break
            else:
                carry = n - limit if n > limit else 0
                seen[(n, carry)] = counter

        elif n - carry < limit:
            new_limit = n - carry
            if carry < new_limit:
                counter += 1
            stack.append([n, carry, limit, counter])
            limit = new_limit
            n = carry
            carry = n - limit if n > limit else 0
            counter = 0

    return counter

#for i in range(5, 20):
#    print(i, solution(i, True), solution(i, False))

print(solution(200))
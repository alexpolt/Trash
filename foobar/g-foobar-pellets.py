
def solution(n, seen = {1: 0}):
    # Your code here
    num = long(n)
    if num in seen:
        return seen[num]
    up = down = div = 2**30
    if num & 1 == 0:
        div = solution(num // 2, seen)
    else:
        down = solution(num - 1, seen)
        up = solution(num + 1, seen)
    min_count = min(div, up, down) + 1
    seen[num] = min_count
    return min_count

def solution1(n, seen = {1: 0}):
    global frame

    num = long(n)
    stack = []
    minops = 0
    frame = {"next": "start"}

    def save(nextop, minops_dec=2**30):
        global frame
        stack.append({"next": nextop, "num": num, "minops_dec": minops_dec})
        frame = {"next": "start"}

    def load():
        global frame
        if not stack:
            frame = {"next": "stop", "num": num}
        else:
            frame = stack.pop()
        return frame["num"]

    while True:
        if frame["next"] == "stop":
            break

        elif frame["next"] == "ret":
            minops = min(frame["minops_dec"], minops) + 1
            seen[num] = minops
            num = load()

        elif frame["next"] == "inc":
            save("ret", minops)
            num = num + 1

        elif num in seen:
            minops = seen[num]
            num = load()

        elif num & 1 == 0:
            save("ret")
            num = num // 2
            
        else:
            save("inc")
            num = num - 1
    return minops

print(solution1("3"))
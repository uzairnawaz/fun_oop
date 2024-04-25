a = fun (int, int, int, int) -> (int) {
    return it2 * it1 * it0 * it3
}

print a(30, 20, 30, 50)

class Adder {
    func3 = fun (int, int, int) -> (int) {
        return it0 + it1 + it2
    }

    func2 = fun (int, int) -> (int) {
        return it0 + it1
    }

    func1 = fun (int, int, int) -> (int) {
        return it0
    }

    func0 = fun (int, int, int) -> (int) {
        print 213213456
        return 3
    }
}

Adder obj = new Adder()
print obj.func3(1, 2, 3)
print obj.func2(1, 2)
print obj.func1(1)
print obj.func0()

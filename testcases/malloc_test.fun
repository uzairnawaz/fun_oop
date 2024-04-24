
class Apple {
    init = fun {
        self.weight = it0
    }
    weight = 5
}
Apple b = new Apple(2)
a = b.init(2)
print b.weight

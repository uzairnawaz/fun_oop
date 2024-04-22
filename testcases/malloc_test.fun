
class Apple {
    init = fun {
        self.weight = 24
    }
    weight = 5
    color = 0
}
Apple a = new Apple(5)
Apple b = a
Apple c
a.weight = 3
a.color = 10

print a.weight
print a.color
print b.weight
print b.color

b = new Apple(2)
print b.weight
print b.color
print a.weight
print a.color

b.init(10)
print b.weight


class Animal {
    size = 20
    x = fun {
        self.size = self.size + 1
        return self.size
    }
}

Animal[] animals = new Animal[20]

Animal y = animals[10]
y.x(0)
print animals[10].x(9)
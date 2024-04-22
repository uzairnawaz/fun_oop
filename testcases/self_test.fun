
class Dog {
    age = 1
    size = 4
    makeBigger = fun {
        self.size = self.size + it
    }
    bark = fun {
        print 9999 
    }
}

class Cat {
    age = 5
    Dog enemy = new Dog(0)
    Cat friend
    makeOlder = fun {
        self.age = self.age + it
    }
}


Cat c = new Cat(0)

print c.age
c.makeOlder(3)
print c.age

print c.enemy.age
print c.enemy.size
c.enemy.makeBigger(2)
print c.enemy.size
c.enemy.bark(0)
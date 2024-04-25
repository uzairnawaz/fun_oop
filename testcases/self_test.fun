
class Dog {
    age = 1
    size = 4
    makeBigger = fun (int) -> (int) {
        self.size = self.size + it0
    }
    bark = fun () -> (int) {
        print 9999 
    }
}

class Cat {
    age = 5
    Dog enemy = new Dog()
    Cat friend
    makeOlder = fun (int) -> (int) {
        self.age = self.age + it0
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
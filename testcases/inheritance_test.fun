
class Organism extends Object {
    age = 0
    isMulticelled = 0
    speak = fun {
        print 1000
    }
    ageInMonths = fun {
        return 12 * self.age
    }
}

class Animal extends Organism {
    canSwim = 0
    isMulticelled = 1
    speak = fun {
        print 2000
    }
    makeOlder = fun {
        self.age = self.age + 1
    }
    doesBark = 0
}

class Dog extends Animal {
    canSwim = 1
    doesBark = 1
    bark = fun {
        print 3000
    }
    speak = fun {
        print 7000
    }
}

class Cat extends Animal {
    meow = fun {
        print 4000
    }
}

Cat c = new Cat(0)
Dog d = new Dog(0)
Animal a = new Animal(0)
Organism o = new Organism(0)

print o.age
print o.isMulticelled
o.speak(0)
print o.ageInMonths(0)

print a.age
print a.isMulticelled
a.speak(0)
print a.canSwim
a.makeOlder(0)
print a.ageInMonths(0)

print d.age
print d.canSwim
d.bark(0)
print d.doesBark

print c.doesBark
c.meow(0)
print c.canSwim

Animal a = new Dog(0)
print a.canSwim
a.speak(0)

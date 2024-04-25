
class Organism extends Object {
    age = 0
    isMulticelled = 0
    speak = fun () -> (int) {
        print 1000
    }
    ageInMonths = fun () -> (int) {
        return 12 * self.age
    }
}

class Animal extends Organism {
    canSwim = 0
    isMulticelled = 1
    speak = fun () -> (int) {
        print 2000
    }
    makeOlder = fun () -> (int) {
        self.age = self.age + 1
    }
    doesBark = 0
}

class Dog extends Animal {
    canSwim = 1
    doesBark = 1
    bark = fun () -> (int) {
        print 3000
    }
    speak = fun () -> (int) {
        print 7000
    }
}

class Cat extends Animal {
    meow = fun () -> (int) {
        print 4000
    }
}

Cat c = new Cat()
Dog d = new Dog()
Animal a = new Animal()
Organism o = new Organism()

print o.age
print o.isMulticelled
o.speak(0)
print o.ageInMonths()

print a.age
print a.isMulticelled
a.speak()
print a.canSwim
a.makeOlder()
print a.ageInMonths()

print d.age
print d.canSwim
d.bark()
print d.doesBark

print c.doesBark
c.meow()
print c.canSwim

Animal a = new Dog()
print a.canSwim
a.speak()

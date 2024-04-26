#include testcases/lib/HashMap.fun

class Dog {
    paw = 4
}

Dog a = new Dog()
Dog b = new Dog()

a.paw = 1
b.paw = 2

HashMap map = new HashMap()
map.init()

map.put(104, a)
map.put(104, b)
map.put(3, a)
map.put(1000, b)

Dog ret = map.get(104)
print ret.paw
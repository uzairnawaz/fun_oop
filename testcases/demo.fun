#include testcases/lib/HashMap.fun

class Person {
    private listenToTravisScott = fun () -> (int) {
        printc 'g'
        printc 'o'
        printc 'o'
        printc 's'
        printc 'e'
        printc 'b'
        printc 'u'
        printc 'm'
        printc 'p'
        printc 's'
        printc 10
    }

    public listenToTaylorSwift = fun () -> (int) {
        printc 's'
        printc 'h'
        printc 'a'
        printc 'k'
        printc 'e'
        printc 32
        printc 'i'
        printc 't'
        printc 32
        printc 'o'
        printc 'f'
        printc 'f'
        printc 13
        printc 10
    }
}

Person gheith = new Person()
Person chrill = new Person()

HashMap map = new HashMap()
map.init()

map.put(104, chrill)
map.put(104, gheith)
map.put(3, gheith)
map.put(1000, chrill)

Person person = map.get(104)
person.listenToTaylorSwift()
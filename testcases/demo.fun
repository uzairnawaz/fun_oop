#include testcases/lib/HashMap.fun

class Person {
    private listenToTravisScott = fun () -> (int) {
        printc 103
        printc 111
        printc 111
        printc 115
        printc 101
        printc 98
        printc 117
        printc 109
        printc 112
        printc 115
        printc 10
    }

    public listenToTaylorSwift = fun () -> (int) {
        printc 115
        printc 104
        printc 97
        printc 107
        printc 101
        printc 32
        printc 105
        printc 116
        printc 32
        printc 111
        printc 102
        printc 102
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
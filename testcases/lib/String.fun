#include testcases/lib/ArrayList.fun

class String {
    ArrayList str = new ArrayList()
    len = 0

    addChar = fun (int) -> (int) {
        self.str.add(it0)
        self.len = self.len + 1
    }

    display = fun (int) -> (int) {
        displayloc = 0
        while (displayloc < self.len) {
            printc self.str.get(displayloc)
            displayloc = displayloc + 1
        }
    }

    size = fun (int) -> (int) {
        return self.len
    }

}
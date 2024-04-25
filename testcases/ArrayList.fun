class ArrayList {
    size = 10
    pos = 0
    int[] arr = new int[self.size]
    
    add = fun (int) -> (int) {

        if (self.pos == self.size) {
            self.pos = 0
            Animal d = new Animal()
        }

        else {
            self.arr[self.pos] = it0
            self.pos = self.pos + 1
        }
    }

    get = fun (int) -> (int) {
        return self.arr[it0]
    }
}

class Animal {
    dog = 5
}
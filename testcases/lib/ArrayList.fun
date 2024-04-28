class ArrayList {
    size = 2
    pos = 0
    int[] arr = new int[self.size]

    add = fun (int) -> (int) {
        if (self.pos == self.size) {
            int[] newArr = new int[self.size * 2]
            i = 0
            while (i < self.size) {
                newArr[i] = self.arr[i]
                i = i + 1
            }

            newArr[self.pos] = it0
            self.arr = newArr

            self.size = self.size * 2
        }
        else {
            self.arr[self.pos] = it0
        }
        self.pos = self.pos + 1
    }

    get = fun (int) -> (int) {
        return self.arr[it0]
    }
}


class Abc {
    idk = 2
    idk2 = 4
    idkFunc = fun {
        return self.idk + self.idk2
    }
}

class Animal {
    size = 10
    height = 4
    randomFunc = fun {
        return self.size * self.height
    }
    Abc abc = new Abc(0)
}

Animal[] arr = new Animal[4]

i = 0
while (i < 4) {
    print arr[i].size
    print arr[i].height
    print arr[i].randomFunc(0)
    print arr[i].abc.idk
    print arr[i].abc.idk2
    print arr[i].abc.idkFunc(0)
    i = i + 1
}

arr[1].size = 20
arr[2].size = 30
i = 0
while (i < 4) {
    print arr[i].size
    i = i + 1
}
arr[1] = new Animal(0)
i = 0
while (i < 4) {
    print arr[i].size
    i = i + 1
}
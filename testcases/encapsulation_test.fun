class Building {
    height = 4
    capacity = 10
    private x1 = fun () -> (int) {
        return self.height
    }
    x2 = fun () -> (int) {
        return self.x1()
    }
}

class ConcertHall extends Building {
    roomCount = 3
    capacity = 100
    height = 5

}

Building x = new Building()
print x.x1()


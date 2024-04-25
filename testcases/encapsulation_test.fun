class Building {
    private height = 4
    capacity = 10
    private x1 = fun {
        return self.height
    }
    x2 = fun {
        return self.x1(0)
    }
}

class ConcertHall extends Building {
    roomCount = 3
    capacity = 100

}

Building x = new Building(0)
print x.x2(0)



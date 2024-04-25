
class Color {
    r = 10
    g = 20
    b = 30
}

class Food {
    calories = 200
    Color c = new Color()
}

class Animal {
    age = 0
    caloriesNeeded = 2000
    eat = fun (Food) -> (int) {
        self.caloriesNeeded = self.caloriesNeeded - it0.calories
    }
}

Animal a = new Animal()
Food f = new Food()
Color c = new Color()

print a.caloriesNeeded
a.eat(f)
print a.caloriesNeeded
a.age = 5

Animal returnFirst = fun (Animal, Color, Food) -> (Animal) {
    print it0.age
    return it0
}

Color returnSecond = fun (Animal, Color, Food) -> (Color) {
    print it1.r
    return it1
}

Food returnThird = fun (Animal, Color, Food) -> (Food) {
    print it2.calories
    return it2
}


print returnFirst(a, c, f).age
print returnSecond(a, c, f).g
print returnThird(a, c, f).calories
print returnThird(a, c, f).c.b

Color argumentTypeTest = fun (Food) -> (Color) {
    print it0.calories
    Color inside = fun (Color) -> (Color) {
        print it0.r
        return c
    }
    print it0.calories
    Color out = inside(c)
    print it0.calories
    return out
}
print argumentTypeTest(f).b


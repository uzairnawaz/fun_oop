#include testcases/ArrayList.fun

ArrayList myArr = new ArrayList()

i = 0
while (i < 9) {
    myArr.add(i)
    i = i + 1
}

i = 0
while (i < 9) {
    print myArr.get(i)
    i = i + 1
}

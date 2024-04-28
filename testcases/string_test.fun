#include testcases/lib/String.fun

String s = new String()

letter = 97
while (letter < 97 + 26) {
    s.addChar(letter)
    letter = letter + 1
}


s.display()

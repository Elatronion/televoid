import "televoid/core" for Inventory

Inventory.add(2)

if (Inventory.has(2)) {
  System.print("We Have Item 2")
} else {
  System.print("We Do NOT Have Item 2")
}

/*
var a = 1
var b = 1
for (i in 1..15) {
  System.print(a)
  var z = a + b
  a = b
  b = z
}
*/

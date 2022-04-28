function Person(a) {
    this.age = a;
    var age = a;
}  
var z = new Person(18);
z.xxx = z.age + 120;

function outer() {
    var x = 1;
    function middle() {
        function inner() {
            return x + 2;
        }
        return inner;
    }
    return middle;
}

var mid = outer();
var inner = mid();
var z = inner();
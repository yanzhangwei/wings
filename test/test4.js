var c = {
    z: 1,
    x: 3,
    v: function() {
        var y = 10;
        return this.z + this.x;
    }
};
var z = c.z - c.x * c.v();
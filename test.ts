// tests go here; this will not be compiled when this package is used as an extension.
let val = 0
basic.forever(function () {
    val = custom.bar(val)
    basic.showNumber(val)
})
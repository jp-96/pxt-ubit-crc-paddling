// tests go here; this will not be compiled when this package is used as an extension.
input.onButtonPressed(Button.A, function () {
    custom.countUpCrankRevolutions();
})
input.onButtonPressed(Button.B, function () {
    custom.countUpCrankRevolutions();
    custom.countUpCrankRevolutions();
    custom.countUpCrankRevolutions();
})
custom.startCSCService()

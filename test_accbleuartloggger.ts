bluetooth.onBluetoothConnected(function () {
    basic.showIcon(IconNames.Happy)
})
bluetooth.onBluetoothDisconnected(function () {
    basic.showIcon(IconNames.No)
})
input.onButtonPressed(Button.A, function () {
    if (実行中 != 1) {
        実行中 = 1
        カウントダウン = 5
        while (カウントダウン > 0 && 実行中 == 1) {
            basic.showNumber(カウントダウン)
            カウントダウン += -1
            basic.pause(200)
        }
        basic.showIcon(IconNames.Heart)
        bluetooth.uartWriteLine(" BEGIN")
        while (実行中 == 1) {
            加速度g = input.acceleration(Dimension.Strength)
            加速度x = input.acceleration(Dimension.X)
            加速度y = input.acceleration(Dimension.Y)
            加速度z = input.acceleration(Dimension.Z)
            加速度データ = " g " + convertToText(加速度g) + " x " + convertToText(加速度x)
            bluetooth.uartWriteLine(加速度データ)
            加速度データ = " y " + convertToText(加速度y) + " z " + convertToText(加速度z)
            bluetooth.uartWriteLine(加速度データ)
            basic.pause(0)
        }
        bluetooth.uartWriteLine(" END")
        basic.showIcon(IconNames.Happy)
    }
})
input.onButtonPressed(Button.B, function () {
    実行中 = 0
})
let 加速度データ = ""
let 加速度z = 0
let 加速度y = 0
let 加速度x = 0
let 加速度g = 0
let カウントダウン = 0
let 実行中 = 0
bluetooth.startUartService()
basic.showIcon(IconNames.No)

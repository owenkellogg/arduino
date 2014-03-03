//var serialPort = require("serialport");
//serialPort.list(function (err, ports) {
  //ports.forEach(function(port) {
    //console.log(port.comName);
    //console.log(port.pnpId);
    //console.log(port.manufacturer);
  //});
//});

var serialport = require("serialport"),
    SerialPort = serialport.SerialPort;

var arduino = new SerialPort("/dev/cu.usbmodem1411", {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
});

arduino.on("open", function () {
  console.log('open');
  arduino.on('data', function(data) {
    process.stdout.write("\r" + data + "     ");
  });
});

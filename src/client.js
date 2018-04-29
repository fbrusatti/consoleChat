var net = require('net');
var readline = require('readline');

var HOST = '127.0.0.1';
var PORT = 4848;

var client = new net.Socket();
client.connect(PORT, HOST, function() {
  console.log('Connected...');
});

client.on('data', function(data) {
 console.log('>>> ' + data);
});

client.on('drain', function(data) {
  console.log('drained buffer');
});

client.on('end', () => {
  console.log('disconnected from server');
  process.exit();
});

/* READ FROM STDIN */
var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.on('line', (input) => {
  client.write(input);
});


// process.stdin.on('data', function(data) {
//   if (data == 'close')
//     client.close();
//   client.write(data);
// });

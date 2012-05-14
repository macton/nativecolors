
var file    = new Stream( system.arguments[0], 'r' );
var tu_text = file.readFile();
file.close();

var lines = tu_text.split(/\n+/);
var i;
var nodes = new Object();
var node;
var j;

for (i=0;i<lines.length;i++) {
  var line   = lines[i];
  line.replace(/:/g,'');
  var fields = line.split(/\s+/);
  if ( fields[0].charAt(0) == '@' ) {
    var id = fields.shift();
    var type = fields.shift();
    node = new Object();
    nodes[ id ] = node;
    node.type = type;
  }
  for (j=0;j<fields.length;j+=2) {
    node[ fields[j] ] = fields[j+1];
  }
}

writeln( JSON.stringify( nodes ) );


NativeBlueTerm = function( terminal_id, nmf ) {

  hterm.NativeBlue = function(argv) {
    this.argv_        = argv;
    this.environment_ = argv.environment || {};
    this.io           = null;
    this.verbose_     = false;
    this.relay_       = null;
    this.alertDialog  = new AlertDialog(window.document.body);
    this.promptDialog = new PromptDialog(window.document.body);
  };
  
  hterm.NativeBlue.init = function() {
    var profileName = hterm.parseQuery(document.location.search)['profile'];
    var terminal    = new hterm.Terminal(profileName);
  
    terminal.decorate(document.querySelector(terminal_id));
  
    // is this used internally by hterm?
    window.term_ = terminal;
  
    // Need to look at how call is used to find a better place to store the nmf.
    window.nmf   = nmf;
  
    terminal.setCursorPosition(0, 0);
    terminal.setCursorVisible(false);
    terminal.runCommandClass(hterm.NativeBlue);
  };
  
  hterm.NativeBlue.prototype.commandName = 'nativeblue';
  
  hterm.NativeBlue.prototype.run = function() {
    this.io  = this.argv_.io.push();
    var self = this;
  
    this.initPlugin( function() {
    self.io.println('READY.');
    });
  };
  
  hterm.NativeBlue.prototype.initPlugin = function( on_complete ) {
    var self = this;
  
    function onPluginLoaded() {
      // self.io.println('PLUGIN_LOADING_COMPLETE.');
      on_complete();
    };
  
    // this.io.println('PLUGIN LOADING...');
  
    this.plugin = window.document.createElement('embed');
    this.plugin.style.cssText =
        ('position: absolute;' +
         'top: -99px' +
         'width: 0;' +
         'height: 0;');
    this.plugin.setAttribute('src', window.nmf );
    this.plugin.setAttribute('type', 'application/x-nacl');
    this.plugin.addEventListener('load', onPluginLoaded);
    this.plugin.addEventListener('message', this.onPluginMessage.bind(this));
  
    document.body.insertBefore(this.plugin, document.body.firstChild);
  };
  
  hterm.NativeBlue.prototype.onPluginMessage = function(msg) {
    var self = this;
    var info = JSON.parse( msg.data );
  
    // #todo create a standard function for nativeblue that forwards all the functions
    //       to the appropriate file
  
    if ( info.func == 'write' ) {
      if ( info.dev == '/dev/stdout' ) {
        self.io.println( decodeURIComponent( info.data ) );
      }
    }
  };

  //
  // Create the terminal
  //

  hterm.init( hterm.NativeBlue.init );
};

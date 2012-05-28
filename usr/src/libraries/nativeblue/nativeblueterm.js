
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
    var self = this;

    self.io         = self.argv_.io.push();
    self.terminal   = self.io.terminal_;
    self.scrollPort = self.terminal.scrollPort_;

    function onPluginLoaded() {
      // self.io.println('PLUGIN_LOADING_COMPLETE.');

      self.io.onTerminalResize = self.onTerminalResize.bind(self);
    };
  
    // self.io.println('PLUGIN LOADING...');
  
    self.plugin = window.document.createElement('embed');
    self.plugin.style.cssText =
        ('position: absolute;' +
         'top: -99px' +
         'width: 0;' +
         'height: 0;');
    self.plugin.setAttribute('src', window.nmf );
    self.plugin.setAttribute('type', 'application/x-nacl');
    self.plugin.addEventListener('load', onPluginLoaded);
    self.plugin.addEventListener('message', self.onPluginMessage.bind(self));

  
    document.body.insertBefore(self.plugin, document.body.firstChild);
  };
  
  hterm.NativeBlue.prototype.onPluginMessage = function(msg) {
    var self = this;
    var info = JSON.parse( msg.data );
  
    // #todo create a standard function for nativeblue that forwards all the functions
    //       to the appropriate file
  
    if ( info.func == 'write' ) {
      if ( info.dev == '/dev/stdout' ) {
        self.terminal.interpret( decodeURIComponent( info.data ) );
      }
    }
  };

  hterm.NativeBlue.prototype.onTerminalResize = function(width, height) {
    // var str = JSON.stringify({name: name, arguments: arguments});
    // this.plugin_.postMessage(str);
  };

  //
  // Create the terminal
  //

  window.webkitStorageInfo.requestQuota( PERSISTENT, 8 * 1024 * 1024 * 1024, function(grantedBytes) {
    hterm.init( hterm.NativeBlue.init );
  });
};

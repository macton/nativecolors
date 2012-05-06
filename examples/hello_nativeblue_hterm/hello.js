
window.onload = function() {
    hterm.init(hterm.Hello.init);
};

hterm.Hello = function(argv) {
  this.argv_        = argv;
  this.environment_ = argv.environment || {};
  this.io           = null;
  this.verbose_     = false;
  this.relay_       = null;
  this.alertDialog  = new AlertDialog(window.document.body);
  this.promptDialog = new PromptDialog(window.document.body);
};

hterm.Hello.init = function() {
  var profileName = hterm.parseQuery(document.location.search)['profile'];
  var terminal    = new hterm.Terminal(profileName);

  terminal.decorate(document.querySelector('#terminal'));

  window.term_ = terminal;

  terminal.setCursorPosition(0, 0);
  terminal.setCursorVisible(false);
  terminal.runCommandClass(hterm.Hello);
};

/**
 * The name of this command used in messages to the user.
 *
 * Perhaps this will also be used by the user to invoke this command, if we
 * build a shell command.
 */
hterm.Hello.prototype.commandName = 'hello';

/**
 * Start the hello command.
 *
 * This is invoked by the terminal as a result of terminal.runCommandClass().
 */
hterm.Hello.prototype.run = function() {
  this.io  = this.argv_.io.push();
  var self = this;

  this.initPlugin( function() {
    // self.io.println('READY.');
  });
};

hterm.Hello.prototype.initPlugin = function( on_complete ) {
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
  this.plugin.setAttribute('src', 'hello.nmf');
  this.plugin.setAttribute('type', 'application/x-nacl');
  this.plugin.addEventListener('load', onPluginLoaded);
  this.plugin.addEventListener('message', this.onPluginMessage.bind(this));

  document.body.insertBefore(this.plugin, document.body.firstChild);
};

hterm.Hello.prototype.onPluginMessage = function(msg) {
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

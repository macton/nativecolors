
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

  self.io.println('\033[32;40mHello, World!');
};

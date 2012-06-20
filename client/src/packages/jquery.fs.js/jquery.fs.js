(function( $ ){

  $.fs = function() {

    var filer = new Filer();

    var fs_interface = {

      /* Constants */

      DEFAULT_FS_SIZE:                                   Filer.DEFAULT_FS_SIZE,

      /* API */

      isOpen: function()                                 { return filer.isOpen; },
      size:   function()                                 { return filer.size; },
      type:   function()                                 { return filer.type; },
      init:   function( config )                         { return bindPromise( filer, filer.init, (config === undefined)?{}:config ); },
      mkdir:  function( path, exists_error )             { return bindPromise( filer, filer.mkdir,  path, (exists_error === undefined)?false:exists_error ); },
      create: function( path, exists_error )             { return bindPromise( filer, filer.create, path, (exists_error === undefined)?false:exists_error ); },
      rm:     function( path )                           { return bindPromise( filer, filer.rm, path ); },
      ls:     function( path )                           { return bindPromise( filer, filer.ls, path ); },
      cd:     function( path )                           { return bindPromise( filer, filer.cd, path ); },
      cp:     function( source, dest_folder, dest_name ) { return bindPromise( filer, filer.cp, source, dest_folder, (dest_name === undefined)?null:dest_name ); },
      mv:     function( source, dest_folder, dest_name ) { return bindPromise( filer, filer.mv, source, dest_folder, (dest_name === undefined)?null:dest_name ); },
      open:   function( path )                           { return bindPromise( filer, filer.open, path ); },
      write:  function( path, content )                  { return bindPromise( filer, filer.write, path, $.isFunction(content)?content(path):content ); }, 

      /* Helpers */
       
      root:                function()       { return filer.fs.root; },
      pathToFilesystemURL: function( path ) { return filer.pathToFilesystemURL( path ); },       

      /* Utility methods */

      fileToObjectURL:           Util.fileToObjectURL,
      fileToArrayBuffer:         function( blob ) { return bindPromise( window, Util.fileToArrayBuffer, blob ); },
      arrayBufferToBlob:         Util.arrayBufferToBlob,
      arrayBufferToBinaryString: function( buffer ) { return bindPromise( window, Util.arrayBufferToBinaryString, buffer ); },
      strToObjectURL:            Util.strToObjectURL,
      strToDataURL:              Util.strToDataURL,
      arrayToBinaryString:       Util.arrayToBinaryString,
      getFileExtension:          Util.getFileExtension,
      dataURLToBlob:             Util.dataURLToBlob,
      toArray:                   Util.toArray
    };

    function bindPromise() {
      var deferred = $.Deferred();
      var params   = Array.prototype.slice.call(arguments);
      var that     = params.shift();   
      var fn       = params.shift();   
      var promise  = deferred.promise();
 
      fn.apply( that, params.concat( [ deferred.resolve, deferred.reject ] ) );

      function bindPromiseInterface( self, fn, args ) {
        var pipe = self.pipe( function() { return fs_interface[fn].apply( fs_interface, args ); } );
        return $.extend( pipe, chain_interface );
      }

      var chain_interface = {
        init:   function() { return bindPromiseInterface( this, 'init',   arguments ); },
        mkdir:  function() { return bindPromiseInterface( this, 'mkdir',  arguments ); },
        create: function() { return bindPromiseInterface( this, 'create', arguments ); },
        rm:     function() { return bindPromiseInterface( this, 'rm',     arguments ); },
        ls:     function() { return bindPromiseInterface( this, 'ls',     arguments ); },
        cd:     function() { return bindPromiseInterface( this, 'cd',     arguments ); },
        cp:     function() { return bindPromiseInterface( this, 'cp',     arguments ); },
        mv:     function() { return bindPromiseInterface( this, 'mv',     arguments ); },
        open:   function() { return bindPromiseInterface( this, 'open',   arguments ); },
        write:  function() { return bindPromiseInterface( this, 'write',  arguments ); }
      };
 
      return $.extend( promise, chain_interface );
    }
  
    return fs_interface;
  }

})( jQuery );



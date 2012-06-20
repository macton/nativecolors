function onError(e) {
  ok(false, 'unexpected error ' + e.name);
  start();
};

function onError(e) {
  ok(false, 'unexpected error ' + e.name);
  start();
};

module('init', {
  setup: function() {
    this.fs = $.fs();
  },
  teardown: function() {
  }
});

asyncTest('init persistent', function() {
  var fs = this.fs;

  fs.init({ persistent: true })
    .done( function() {
      equal(fs.isOpen(), true,            'filesystem open');
      equal(fs.type(),   this.PERSISTENT, 'type PERSISTENT');
      start();
    })
    .fail( onError );
});

asyncTest('init persistent 8GB', function() {
  var fs = this.fs;
  var size  = 8 * 1024 * 1024 * 1024;

  fs.init({ persistent: true, size: size })
    .done( function() {
      equal(fs.isOpen(), true,            'filesystem open');
      equal(fs.type(),   this.PERSISTENT, 'type PERSISTENT');
      equal(fs.size(),   size,            'alloc 8GB');
      start();
    })
    .fail( onError );
});

asyncTest('init temp', function() {
  var fs = this.fs;

  fs.init({ persistent: false})
    .done( function() {
      equal(fs.isOpen(), true,            'filesystem open');
      equal(fs.type(),   this.TEMPORARY,  'type TEMPORARY');
      start();
    })
    .fail( onError );
});

asyncTest('init temp 8GB', function() {
  var fs = this.fs;
  var size  = 8 * 1024 * 1024 * 1024;

  fs.init({ persistent: false, size: size })
    .done( function() {
      equal(fs.isOpen(), true,            'filesystem open');
      equal(fs.type(),   this.TEMPORARY,  'type TEMPORARY');
      equal(fs.size(),   size,            'alloc 8GB');
      start();
    })
    .fail( onError );
});

module('mkdir', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
    // WTF? Called before all the tests in the module are complete?
    // stop();
    // this.fs.rm(this.FOLDER_NAME).then( start, onError );
  }
});

asyncTest('mkdir', function() {
  var fs = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir(folder_name, false)
    .done( function(entry) {
      ok(entry.isDirectory, 'created folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name, 'created folder is named "' + folder_name + '"');
      start();
    })
    .fail(onError);
});

asyncTest('mkdir-undefined-exists-error', function() {
  var fs = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir(folder_name)
    .then( function(entry) {
      ok(entry.isDirectory, 'created folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name, 'created folder is named "' + folder_name + '"');
      start();
    })
    .fail( function(e) {
      ok(false, "Default exclusive parameter is not false");
      start();
    });
});

asyncTest('mkdir-already-exists-error', function() {
  var fs          = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir(folder_name, true)
    .then( function(entry) {
      ok(false);
      start();
    })
    .fail( function(e) {
      ok(true, "Attempted to create a folder that already exists");
      start();
    });

});

asyncTest('mkdir-new-sub-path', function() {
  var fs              = this.fs;
  var folder_name     = this.FOLDER_NAME + Date.now();
  var folder_name_sub = folder_name + '/1/2/3/4/5/6/7/8/9/A/B/C/D/E/F';

  fs.mkdir(folder_name_sub)
    .done( function(entry) {
      equal(entry.fullPath, folder_name_sub, "Subfolders created properly");
      start();
    })
    .fail(onError);
});

module('rm', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

/* -------------------------------------------------
   Known to fail:
   https://github.com/ebidel/filer.js/issues/15
   -------------------------------------------------
asyncTest('rm-new-sub-path', function() {
  var fs              = this.fs;
  var folder_name     = this.FOLDER_NAME + Date.now();
  var folder_name_sub = folder_name + '/1/2/3/4/5/6/7/8/9/A/B/C/D/E/F';

  fs.mkdir( folder_name_sub )
    .pipe( function() { return fs.rm( folder_name_sub );  } )
    .pipe( function() { return fs.mkdir( folder_name_sub, true )
                               .then( function() { 
                                 ok( true, 'Could make unique directory after removed'); 
                                 start();
                               }); 
    })
    .fail( onError );
});
*/

module('cd', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('cd', function() {
  var fs          = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir( folder_name )
    .pipe( function() { return fs.mkdir( folder_name + '/1' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/2' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/3' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/4' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/5' ); } )
    .pipe( function() { return fs.cd( folder_name ); } )
    .done( function( entry ) { 
      ok(entry.isDirectory, 'created folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name, 'created folder is named "' + entry.fullPath + '"');
    })
    .pipe( function() { return fs.cd('1'); } )
    .done( function( entry ) {
      ok(entry.isDirectory, 'created sub folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name + '/1', 'created sub folder is named "' + entry.fullPath + '"');
      start();
    })
    .fail( onError );
});

asyncTest('cd-chain', function() {
  var fs          = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir( folder_name )
    .mkdir( folder_name + '/1' )
    .mkdir( folder_name + '/2' )
    .mkdir( folder_name + '/3' )
    .mkdir( folder_name + '/4' )
    .mkdir( folder_name + '/5' )
    .cd( folder_name )
    .done( function( entry ) { 
      ok(entry.isDirectory, 'created folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name, 'created folder is named "' + entry.fullPath + '"');
    })
    .cd('1')
    .done( function( entry ) {
      ok(entry.isDirectory, 'created sub folder is a DirectoryEntry');
      equal(entry.fullPath, folder_name + '/1', 'created sub folder is named "' + entry.fullPath + '"');
      start();
    })
    .fail( onError );
});


module('ls', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('ls-count', function() {
  var fs          = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir( folder_name )
    .pipe( function() { return fs.mkdir( folder_name + '/1' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/2' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/3' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/4' ); } )
    .pipe( function() { return fs.mkdir( folder_name + '/5' ); } )
    .pipe( function() { return fs.ls( folder_name ) } )
    .done( function( entries ) { 
      ok(entries.slice, 'returned entries is an array') // Verify we got an Array.
      equal(entries.length, 5, 'Num root entries matches');
      start();
    })
    .fail( onError );
});

asyncTest('ls-count-chain', function() {
  var fs          = this.fs;
  var folder_name = this.FOLDER_NAME + Date.now();

  fs.mkdir( folder_name )
    .mkdir( folder_name + '/1' )
    .mkdir( folder_name + '/2' )
    .mkdir( folder_name + '/3' )
    .mkdir( folder_name + '/4' )
    .mkdir( folder_name + '/5' )
    .ls( folder_name )
    .done( function( entries ) { 
        ok(entries.slice, 'returned entries is an array') // Verify we got an Array.
        equal(entries.length, 5, 'Num root entries matches');
        start();
      })
    .fail( onError );
});

module('create', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('create-file', function() {
  var fs        = this.fs;
  var file_name = this.FOLDER_NAME + Date.now();

  fs.create( file_name )
    .done( function( entry ) {
      ok( entry.isFile, 'created file is fileEntry' );
      start();
    })
    .fail( onError );
});

asyncTest('create-file-exists', function() {
  var fs        = this.fs;
  var file_name = this.FOLDER_NAME + Date.now();

  fs.create( file_name )
    .pipe( function() { return fs.create( file_name, true ) } )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to create a file that already exists");
      start();
    });
});


asyncTest('create-file-exists-chain', function() {
  var fs        = this.fs;
  var file_name = this.FOLDER_NAME + Date.now();

  fs.create( file_name )
    .create( file_name, true )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to create a file that already exists");
      start();
    });
});


module('cp', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('cp-same-folder', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var sub_folder  = top_folder + '/1';

  fs.mkdir( sub_folder )
    .done( function() {
      ok( true, "Made new directory to copy" );
    })
    .cp( sub_folder, top_folder )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to copy on top of a directory that already exists");
      start();
    });
});

asyncTest('cp-same-file', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';

  fs.mkdir( top_folder )
    .create( file_name )
    .done( function() {
      ok( true, "Made new file to copy" );
    })
    .cp( file_name, top_folder )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to copy on top of a file that already exists");
      start();
    });
});

asyncTest('cp-dupe-structure', function() {
  var fs               = this.fs;
  var top_folder_name  = this.FOLDER_NAME + Date.now();
  var folder_name      = top_folder_name + '/A';
  var dupe_folder_name = top_folder_name + '/B';
  var ndx;
  var entry;

  fs.mkdir( folder_name )
    .mkdir( folder_name + '/1' )
    .mkdir( folder_name + '/2' )
    .mkdir( folder_name + '/3' )
    .mkdir( folder_name + '/4' )
    .mkdir( folder_name + '/5' )
    .cp( folder_name, top_folder_name, 'B' )
    .ls( dupe_folder_name )
    .done( function( entries ) { 
        ok(entries.slice, 'returned entries is an array') // Verify we got an Array.
        equal(entries.length, 5, 'Num root entries matches');
        for ( var ndx in entries ) {
          entry = entries[ndx];
          ok( entry.isDirectory, 'Copied entry ' + entry.fullPath +' is a DirectoryEntry' );
        }
        start();
      })
    .fail( onError );
});


module('mv', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});


asyncTest('mv-same-folder', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var sub_folder  = top_folder + '/1';

  fs.mkdir( sub_folder )
    .done( function() {
      ok( true, "Made new directory to copy" );
    })
    .mv( sub_folder, top_folder )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to copy on top of a directory that already exists");
      start();
    });
});

asyncTest('mv-same-file', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';

  fs.mkdir( top_folder )
    .create( file_name )
    .done( function() {
      ok( true, "Made new file to copy" );
    })
    .mv( file_name, top_folder )
    .done( function() {
      ok( false );
      start();
    })
    .fail( function() {
      ok(true, "Attempted to copy on top of a file that already exists");
      start();
    });
});

asyncTest('mv-dupe-structure', function() {
  var fs               = this.fs;
  var top_folder_name  = this.FOLDER_NAME + Date.now();
  var folder_name      = top_folder_name + '/A';
  var dupe_folder_name = top_folder_name + '/B';
  var ndx;
  var entry;

  fs.mkdir( folder_name )
    .mkdir( folder_name + '/1' )
    .mkdir( folder_name + '/2' )
    .mkdir( folder_name + '/3' )
    .mkdir( folder_name + '/4' )
    .mkdir( folder_name + '/5' )
    .mv( folder_name, top_folder_name, 'B' )
    .ls( dupe_folder_name )
    .done( function( entries ) { 
        ok(entries.slice, 'returned entries is an array') // Verify we got an Array.
        equal(entries.length, 5, 'Num root entries matches');
        for ( var ndx in entries ) {
          entry = entries[ndx];
          ok( entry.isDirectory, 'Copied entry ' + entry.fullPath +' is a DirectoryEntry' );
        }
        start();
      })
    .fail( onError );
});

module('open', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('open-new', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';

  fs.mkdir( top_folder )
    .create( file_name )
    .open( file_name )
    .done( function( file ) {
      ok(file.__proto__ == File.prototype, 'FileEntry as arg. ' + file_name + ' is a File');
      start();
    })
    .fail( onError );
});


module('write', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

asyncTest('write-new', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';
  var data        = '1234567890';

  var bb = new BlobBuilder();
  bb.append(data);

  fs.mkdir( top_folder )
    .create( file_name )
    .write( file_name, { data: bb.getBlob() } )
    .done( function( file_entry, file_writer ) {
      ok(true, 'data as Blob accepted')
      ok(file_entry.isFile, 'Written file is a FileEntry');
    })
    .open( file_name )
    .done( function( file ) {
      equal(file.size, data.length, 'size of data written is correct');
    })
    .rm( file_name )
    .done( function() { 
      ok(true, 'removed file');
      start();
    })
    .fail( onError );
});

asyncTest('write-new-func', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';
  var data        = '1234567890';
  var bb;

  fs.mkdir( top_folder )
    .create( file_name )
    .done( function() {
      bb = new BlobBuilder();
      bb.append(data);
    })
    .write( file_name, function( path ) { return { data: bb.getBlob() }; } )
    .done( function( file_entry, file_writer ) {
      ok(true, 'data as Blob accepted')
      ok(file_entry.isFile, 'Written file is a FileEntry');
    })
    .open( file_name )
    .done( function( file ) {
      equal(file.size, data.length, 'size of data written is correct');
    })
    .rm( file_name )
    .done( function() { 
      ok(true, 'removed file');
      start();
    })
    .fail( onError );
});

asyncTest('write-new-string', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';
  var data        = '1234567890';

  fs.mkdir( top_folder )
    .create( file_name )
    .write( file_name, { data: data } )
    .done( function( file_entry, file_writer ) {
      ok(true, 'data as string accepted')
      ok(file_entry.isFile, 'Written file is a FileEntry');
    })
    .open( file_name )
    .done( function( file ) {
      equal(file.size, data.length, 'size of data written is correct');
    })
    .rm( file_name )
    .done( function() { 
      ok(true, 'removed file');
      start();
    })
    .fail( onError );
});

asyncTest('write-new-uint8', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';
  var data        = '1234567890';
  var uint8       = new Uint8Array(data.split(''));

  fs.mkdir( top_folder )
    .create( file_name )
    .write( file_name, { data: uint8.buffer } )
    .done( function( file_entry, file_writer ) {
      ok(true, 'data as Uint8Array accepted')
      ok(file_entry.isFile, 'Written file is a FileEntry');
    })
    .open( file_name )
    .done( function( file ) {
      equal(file.size, uint8.length, 'size of data written is correct');
    })
    .rm( file_name )
    .done( function() { 
      ok(true, 'removed file');
      start();
    })
    .fail( onError );
});

asyncTest('write-new-string-append', function() {
  var fs          = this.fs;
  var top_folder  = this.FOLDER_NAME + Date.now();
  var file_name   = top_folder + '/1.txt';
  var data        = '1234567890';

  fs.mkdir( top_folder )
    .create( file_name )
    .write( file_name, { data: data } )
    .write( file_name, { data: data, append: true } )
    .done( function( file_entry, file_writer ) {
      ok(true, 'data as string accepted')
      ok(file_entry.isFile, 'Written file is a FileEntry');
    })
    .open( file_name )
    .done( function( file ) {
      equal(file.size, data.length*2, 'size of data written is correct');
    })
    .rm( file_name )
    .done( function() { 
      ok(true, 'removed file');
      start();
    })
    .fail( onError );
});

module('helpers', {
  setup: function() {
    this.fs               = $.fs();
    this.TEST_FOLDER_NAME = '/tmp/test';
    this.FOLDER_NAME      = '/tmp/test/jquery_fs_test';
    this.SIZE             = 8 * 1024 * 1024 * 1024;

    stop();
    this.fs.init({ persistent: false, size: this.SIZE }).then( start, onError );
  },
  teardown: function() {
  }
});

test('pathToFilesystemURL', function() {
  var fs    = this.fs;
  var fsURL = 'filesystem:' + document.location.origin + '/temporary/';
  var path  = 'test/me';

  equal(fs.pathToFilesystemURL('/'), fsURL, 'root as arg');
  equal(fs.pathToFilesystemURL(fsURL), fsURL, 'filesystem URL as arg');
  equal(fs.pathToFilesystemURL(fsURL + path), fsURL + path, 'filesystem URL as arg2');
  equal(fs.pathToFilesystemURL('/' + path), fsURL + path, 'abs path as arg');
  equal(fs.pathToFilesystemURL('./'), fsURL + './', './ as arg');
  equal(fs.pathToFilesystemURL('./' + path), fsURL + './' + path, './ as arg');
  //equal(fs.pathToFilesystemURL('..'), fsURL + '../', '.. as arg');
  equal(fs.pathToFilesystemURL('../'), fsURL + '/../', '../ as arg');
  equal(fs.pathToFilesystemURL('../' + path), fsURL + '../' + path, '../ as arg');
  //equal(fs.pathToFilesystemURL(path), fsURL + path, 'relative path as arg');
});

module('Utils', {
  setup: function() {
  },
  teardown: function() {
  }
});

test('getFileExtension', function() {
  var fs = $.fs();
  equal(fs.getFileExtension('test'), '', 'no ex');
  equal(fs.getFileExtension('test.txt'), '.txt', 'single char');
  equal(fs.getFileExtension('test.cc'), '.cc', 'double char');
  equal(fs.getFileExtension('test.tar.gz'), '.gz', 'double extension');
  equal(fs.getFileExtension('something/test.mp3'), '.mp3', 'path');
});

test('strToDataURL', function() {
  var fs = $.fs();
  var content = 'body { background: green; }';
  var mimetype = 'text/css';
  var expected = 'data:text/css,body { background: green; }';
  var expectedBin = 'data:text/css;base64,Ym9keSB7IGJhY2tncm91bmQ6IGdyZWVuOyB9';
  equal(fs.strToDataURL(content, mimetype), expectedBin, 'plaintext, no arg specified');
  equal(fs.strToDataURL(content, mimetype, false), expected, 'plaintext, opt arg specified');
  equal(fs.strToDataURL(content, mimetype, true), expectedBin, 'binary, opt arg');
});

test('fileToArrayBuffer', function() {
  var fs = $.fs();
  var data = '0123456780';
  var bb = new BlobBuilder();
  bb.append(data);
  stop();

  fs.fileToArrayBuffer(bb.getBlob())
    .done( function(arrayBuffer) {
      ok(arrayBuffer.__proto__ == ArrayBuffer.prototype, 'Result is an ArrayBuffer');
      equal(arrayBuffer.byteLength, data.length, 'Size matches');
      start();
    })
    .fail(onError);
});

test('dataURLToBlob', function() {
  var fs = $.fs();
  var dataURL = 'data:text/html;base64,VGhpcyBpcyBhIHRlc3QK';
  var blob = fs.dataURLToBlob(dataURL);
  ok(blob.__proto__ == Blob.prototype, 'Result is a Blob');
  equal(blob.size, window.atob('VGhpcyBpcyBhIHRlc3QK').length, 'blob.size');
  equal(blob.type, 'text/html', 'blob.type');

  var dataURL2 = 'data:text/html,<p>Hi there</p>';
  var blob = fs.dataURLToBlob(dataURL2);
  equal(blob.size, '<p>Hi there</p>'.length, 'blob.size');
  equal(blob.type, 'text/html', 'blob.type');
});

test('arrayBufferToBlob', function() {
  var fs = $.fs();
  var len = 10;
  var ab = new ArrayBuffer(len);
  var blob = fs.arrayBufferToBlob(ab);
  ok(blob.__proto__ == Blob.prototype, 'Result is a Blob');
  equal(ab.byteLength, len, 'Size matches');
});

test('arrayBufferToBinaryString', function() {
  var fs = $.fs();
  var len = 10;
  var ab = new ArrayBuffer(len);
  stop();
  fs.arrayBufferToBinaryString(ab)
    .done(function(binStr) {
      ok(binStr.__proto__ == String.prototype, 'Result is a String');
      equal(binStr.length, len, 'Size matches');
      start();
    })
    .fail( onError );
});


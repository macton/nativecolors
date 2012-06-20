jquery.fs.js
=======

jquery.fs.js is a small jQuery plugin which wraps [filer.js](/ebidel/filer.js) [1] to provide:
* Deferred/promise interface 
* Chaining support

Except to allow for the above, the interface is passed through to filer.js to keep this plugin minimal.

* jquery.fs.js - Mike Acton <macton@gmail.com>
* filer.js     - Eric Bidelman <http://ericbidelman.tumblr.com/>

Example of promise interface and (sequential) chaining:
```javascript
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
```

Same example, but using pipe to sequence promises manually:
```javascript
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
```

See [tests.js](/macton/jquery.fs.js/blob/master/test/test.js) for more examples of each function, promises and chaining.

[1]: filer.js is a [well tested](/ebidel/filer.js/tree/master/tests) wrapper library for the [HTML5 Filesystem API](http://dev.w3.org/2009/dap/file-system/pub/FileSystem/),
an API which enables web applications to read and write files and folders to to
its own sandboxed filesystem.

Unlike other wrapper libraries, filer.js takes a different approach
by reusing familiar UNIX commands (`cp`, `mv`, `ls`) for its API. The goal is to
make the HTML5 API more approachable for developers that have done file I/O in
other languages.


Supported Browsers
------------------

* Chrome

The HTML5 Filesystem API is only supported in Chrome. Therefore, the library only works in Chrome.

Although the HTML5 Filesystem polyfill (also by Eric Bidelman) may work for you:
<http://ericbidelman.tumblr.com/post/21649963613/idb-filesystem-js-bringing-the-html5-filesystem-api>

Getting started
=======

1. Required: [filer.js](/ebidel/filer.js)
2. Required: [jQuery](http://jquery.com/)
3. [Exploring the FileSystem APIs](http://www.html5rocks.com/tutorials/file/filesystem/)
4. [The Synchronous FileSystem API for Workers](http://www.html5rocks.com/tutorials/file/filesystem-sync/)
5. ["Using the HTML5 Filesystem API"](http://shop.oreilly.com/product/0636920021360.do),

Usage
-----

The underlying Filesystem API is asynchronous, therefore, the library calls are
mostly asynchronous and jQuery promises are returned.

First, create a `jquery.fs` object:

    var fs = $.fs();

Next, initialize the library:

```javascript
fs.init( {persistent: false, size: 1024 * 1024} )
  .done( function( filesys ) {
    // fs.size() == fs.DEFAULT_FS_SIZE
    // fs.isOpen() == true
  })
  .fail( onError );
```

The only argument is an optional initialization object that can contain two
properties, `persistent` (the type of storage to use) and `size`. The second and
third arguments are a success and error callback, respectively:

A promise is returned which can be handled through the typical interface e.g. done(), when(), fail(), etc.

done()/then() is passed a `LocalFileSystem` object. If you don't initialize
the the filesystem with a size, a default size of `fs.DEFAULT_FS_SIZE` (1MB)
will be used. Thus, the previous call can be simplified to:

```javascript
fs.init()
  .done( function( local_file_system ) { ... } )
  .fail( onError );
```

**Error handling**

Many methods will trigger a deferred reject() call on erroor. It can be a
good idea to setup a global error handler for all methods to use:

```javascript
function onError(e) {
  console.log('Error' + e.name);
}
```

Examples
============

## General rule of thumb

For versatility, the library accepts paths to files or directories as string
arguments (a path) or as filesystem URLs. It also can take the
`FileEntry`/`DirectoryEntry` object representing the file/directory.

ls()
-----

*List the contents of a directory.*

The first arg is a path, filesystem URL, or DirectoryEntry to return the contents
for. A promise is returned. ls() can be chained sequentially.

```javascript
// Pass a path.
fs.ls('/')
  .done( function(entries) {
    // entries in the root directory.
  })
  .fail(onError);

// Pass a path.
fs.ls('.')
  .done( function(entries) {
    // entries in the current working directory.
  })
  .fail(onError);

// Pass a path.
fs.ls('path/to/some/dir/')
  .done( function(entries) {
    // entries in "path/to/some/dir/"
  })
  .fail(onError);

// Pass a filesystem: URL.
var fsURL = fs.fs.root.toURL(); // e.g. 'filesystem:http://example.com/temporary/';
fs.ls(fsURL, function(entries) {
  .done( function(entries) {
    // entries in the root folder.
  })
  .fail(onError);

// Pass a DirectorEntry.
fs.ls(fs.fs.root, function(entries) {
  .done( function(entries) {
    // entries in the root directory.
  })
  .fail(onError);
```

cd()
-----

*Allows you to change into another directory.*

This is a convenience method. When using `cd()`, future operations are treated
relative to the new directory. A promise is returned and success callback is passed the `DirectoryEntry`
changed into. cd() can be chained sequentially.

```javascript
// Passing a path.
fs.cd('/path/to/folder').then( function(dirEntry) {
  ...
}, onError);

// Passing a filesystem: URL.
var fsURL = fs.root().toURL(); // e.g. 'filesystem:http://example.com/temporary/';
fs.cd(fsURL + 'myDir').then( function(dirEntry) {
  // cwd becomes /myDir.
}, onError);

// Passing a DirectoryEntry.
fs.cd(dirEntry).then( function(dirEntry2) {
  // dirEntry == dirEntry2
}, onError);

fs.cd('/path/to/folder'); // Both callbacks are optional.
```

create()
-----

*Creates an empty file.*

`create()` creates an empty file in the current working directory. If you wish
to write data to a file, see the `write()` method. A promise is returned. create() can be chained sequentially.

```javascript
fs.create('myFile.txt', false).then( function(fileEntry) {
  // fileEntry.name == 'myFile.txt'
}, onError);

fs.create('/path/to/some/dir/myFile.txt', true).then( function(fileEntry) {
  // fileEntry.fullPath == '/path/to/some/dir/myFile.txt'
}, onError);

fp.create('myFile.txt'); // Last 3 args are optional.
```

The second (optional) argument is a boolean. Setting it to true throws an error
(i.e. deferred reject) if the file you're trying to create already exists.

mkdir()
-----

*Creates an empty directory.*

```javascript
fs.mkdir('myFolder', false).then( function(dirEntry) {
  // dirEntry.isDirectory == true
  // dirEntry.name == 'myFolder'
}, onError);
```

You can pass `mkdir()` a folder name or a path to create. In the latter,
it behaves like UNIX's `mkdir -p`, creating each intermediate directory as needed.

For example, the following would create a new hierarchy ("music/genres/jazz") in
the current folder:

```javascript
fs.mkdir('music/genres/jazz/', false).then( function(dirEntry) {
  // dirEntry.name == 'jazz' // Note: dirEntry is the last entry created.
}, onError);
```

The second argument to `mkdir()` a boolean indicating whether or not an error
should be thrown if the directory already exists. 

A promise is returned. mkdir() can be chained sequentially.

```javascript
fs.mkdir('music/genres/jazz/').then( function(dirEntry) {
  // dirEntry.name == 'jazz' // second parameter option (default=false)
}, onError);
```

rm()
-----

*Removes a file or directory.*

If you're removing a directory, it is removed recursively. 
A promise is returned. rm() can be chained sequentially.

```javascript
fs.rm('myFile.txt').then( function() {
  ...
}, onError);

fs.rm('/path/to/some/someFile.txt').then( function() {
  ...
}, onError);

var fsURL = fs.pathToFilesystemURL('/path/to/some/directory');
fs.rm(fsURL).then( function() {
  ...
}, onError);

fs.rm(directorEntry).then( function() {
  ...
}, onError);
```

cp()
-----

*Copies a file or directory.*

The first argument to `cp()` is the source file/directory you wish to copy,
followed by the destination folder for the source to be copied into.

A promise is returned. cp() can be chained sequentially.

Note: The src and dest arguments need to be the same type. For example, if pass
a string path for the first argument, the destination cannot be a FileEntry.
It must be a string path (or filesystem URL) as well.

```javascript
// Pass string paths.
fs.cp('myFile.txt', '/path/to/other/folder').then( function(entry) {
  // entry.fullPath == '/path/to/other/folder/myFile.txt'
}, onError);

// Pass filesystem URLs.
var srcFsURL = 'filesystem:http://example.com/temporary/myDir';
var destFsURL = 'filesystem:http://example.com/temporary/anotherDir';
fs.cp(srcFsURL, destFsURL).then( function(entry) {
  // fs.pathToFilesystemURL(entry.fullPath) == 'filesystem:http://example.com/temporary/anotherDir/myDir'
}, onError);

// Pass Entry objects.
fs.cp(srcEntry, destinationFolderEntry).then( function(entry) {
  ...
}, onError);

// Mixing string paths with filesystem URLs work too:
fs.cp(srcEntry.toURL(), '/myDir').then( function(entry) {
  ...
}, onError);
```

If you wish to copy the entry under a new name, specify the third (optional) newName argument:

```javascript
// Copy myFile.txt to myFile2.txt in the current directory.
fs.cp('myFile.txt', '.', 'myFile2.txt').then( function(entry) {
  // entry.name == 'myFile2.txt'
}, onError);
```

mv()
-----

*Moves a file or directory.*

The first argument to move is the source file or directory to move, the second
is a destination directory, and the third is an optional new name for the file/folder
when it is moved.

A promise is returned. mv() can be chained sequentially.

```javascript
// Pass string paths.
fs.mv('path/to/myfile.mp3', '/another/dir').then( function(fileEntry) {
  // fileEntry.fullPath == '/another/dir/myfile.mp3'
}, onError);

// Pass a filesystem URL. This example renames file.txt to somethingElse.txt in
// the same directory.
fs.mv('filesystem:http://example.com/temporary/file.txt', '.', 'somethingElse.txt').then( function(fileEntry) {
  // fileEntry.fullPath == '/somethingElse.txt'
}, onError);

// Pass a FileEntry or DirectoryEntry.
fs.mv(folderEntry, destDirEntry).then( function(dirEntry) {
  // folder is moved into destDirEntry
}, onError);

fs.mv('myFile.txt', './someDir'); // The new name and both callbacks are optional.

```

open()
-----

A promise is returned. open() can be chained sequentially.

A File object is passed to the resolve function on success.

```javascript
// Pass a path.
fs.open('myFile.txt').then( function(file) {
  // Use FileReader to read file.
  var reader = new FileReader();
  reader.onload = function(e) {
    ...
  }
  read.readAsArrayBuffer(file);
}, onError);

// Pass a filesystem URL.
fs.open(fileEntry.toURL()).then( function(file) {
  ...
}, onError);

// Pass a FileEntry.
fs.open(fileEntry).then( function(file) {
  ...
}, onError);
```

write()
-----

*Writes content to a file.*

`write()` takes a `string` (path or filesystem URL) or `FileEntry` as it's first argument.
This is the file to write data to. If the does not exist, it is created. Otherwise,
the file's contents are overwritten if it already exists.

The second argument is an object with three properties:
- `data`: the content to write into the file.
- `type`: optional mimetype of the content.
- `append`: optional true if data should be appended to the file.

The second argument can also be a function which returns an object as described above.
- Note: This is one of the few feature specific to jquery.fs.js not provided by filer.js

The resolve callback for this method is passed the `FileEntry` for the file that
was written to and the `FileWriter` object used to do the writing.

A promise is returned. write() can be chained sequentially.

```javascript
// Write files from a file input.
document.querySelector('input[type="file"]').onchange = function(e) {
  var file = e.target.files[0];
  fs.write(file.name, {data: file, type: file.type}).then( function(fileEntry, fileWriter) {
    ...
  }, onError);
};

// Create a Blob and write it out.
var bb = new BlobBuilder();
bb.append('body { background: red; }');
fs.write('styles.css', {data: bb.getBlob('text/css'), type: 'text/css'}).then(
  function(fileEntry, fileWriter) {
    ...
  },
  onError
);

// Create a typed array and write the ArrayBuffer.
var uint8 = new Uint8Array([1,2,3,4,5]);
fs.write(fileEntry, {data: uint8.buffer}).then(
  function(fileEntry, fileWriter) {
    ...
  },
  onError
);

// Write string data.
fs.write('path/to/file.txt', {data: '1234567890', type: 'text/plain'}).then(
  function(fileEntry, fileWriter) {
    ...
  },
  onError
);

// Append to a file.
fs.write('path/to/file.txt', {data: '1234567890', type: 'text/plain', append: true}).then(
  function(fileEntry, fileWriter) {
    ...
  },
  onError
);

// Create a Blob and write it out. Second parameter as function which returns description.
var bb = new BlobBuilder();
bb.append('body { background: red; }');
fs.write('styles.css', function() { return {data: bb.getBlob('text/css'), type: 'text/css'}; }  ).then(
  function(fileEntry, fileWriter) {
    ...
  },
  onError
);
```

Utility methods
============

The library contains a few utility methods to help you out.

```javascript
$.fs.fileToObjectURL(Blob|File);

// promise returned. cannot be sequentially chained.
$.fs.fileToArrayBuffer(blob).then( function(arrayBuffer) {
  ...
}, onError);

var blob = $.fs.arrayBufferToBlob((new Uint8Array(10)).buffer, opt_contentType);

// promise returned. cannot be sequentially chained.
$.fs.arrayBufferToBinaryString((new Uint8Array(10)).buffer).then( function(binStr) {
  ...
}, onError);

$.fs.strToObjectURL(binaryStr, opt_contentType);

$.fs.strToDataURL(binaryStr, contentType) // e.g. "data:application/pdf;base64,Ym9keSB7IG..."
// For plaintext (non-binary data):
// $.fs.strToDataURL('body { background: green; }', 'text/css', false) == data:text/css,body { background: green; }

$.fs.arrayToBinaryString(bytes); // bytes is an Array, each varying from 0-255.

$.fs.getFileExtension('myfile.txt') == '.txt'

// $.fs.toArray(DOMList/NodeList) == Array
document.querySelector('input[type="file"]').onchange = function(e) {
  $.fs.toArray(this.files).forEach(function(file, i) {
    // blah blah blah.
  });
};
```


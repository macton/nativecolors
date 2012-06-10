waitUntilExists(document, 
  function() {
    document.body.appendChild( document.createTextNode('Javascript loaded.') );
  }
);

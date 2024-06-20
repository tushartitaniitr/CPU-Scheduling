// server.js
const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const fs = require('fs');

const app = express();
app.use(bodyParser.json());

app.post('/runScheduling', (req, res) => {
    const { algorithm, timeQuantum, processes } = req.body;

    let input = `${algorithm}\n${processes.length}\n`;
    processes.forEach(process => {
        input += `${process.id} ${process.arrival} ${process.burst} ${process.priority}\n`;
    });

    if (algorithm === 'P') {
        input += `${timeQuantum}\n`;
    }

    fs.writeFileSync('input.txt', input);

    exec('./scheduling < input.txt', (error, stdout, stderr) => {
        if (error) {
            res.status(500).send(`Error: ${stderr}`);
        } else {
            res.send(stdout);
        }
    });
});

app.listen(3000, () => {
    console.log('Server is running on http://localhost:3000');
});

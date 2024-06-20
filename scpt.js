function runScheduling() {
    const algorithm = document.getElementById('algorithm').value;
    const timeQuantum = document.getElementById('timeQuantum').value;
    const processCount = document.getElementById('processCount').value;

    const processes = [];
    for (let i = 1; i <= processCount; i++) {
        const process = document.getElementById(`process${i}`).children;
        processes.push({
            id: parseInt(process[0].value),
            arrival: parseInt(process[1].value),
            burst: parseInt(process[2].value),
            priority: parseInt(process[3].value)
        });
    }

    const data = {
        algorithm,
        timeQuantum: parseInt(timeQuantum),
        processes
    };

    fetch('http://localhost:3000/runScheduling', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    .then(response => response.text())
    .then(output => {
        document.getElementById('output').innerText = output;
    })
    .catch(error => {
        console.error('Error:', error);
    });
}

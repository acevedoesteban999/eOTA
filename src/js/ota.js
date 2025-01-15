const messageContainer = document.getElementById('messageContainerID');
const message = document.getElementById('messageID');
const spiner = document.getElementById('spinerID');


document.getElementById('otaForm').addEventListener('submit', (event) => {
    event.preventDefault();
    
    const fileInput = document.getElementById('firmware');
    const file = fileInput.files[0];

    if (!file) {
        alert('Please select a firmware file!');
        return;
    }

    if (!file.name.endsWith('.bin')) {
        alert('Please select a .bin file!');
        return;
    }
    messageContainer.className = 'alert loading';
    message.textContent = `OTA Update in Progress ... `;
    spiner.style.display = "block";
    file.arrayBuffer().then(fileData => {
        fetch('/ota_update', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/octet-stream',
            },
            body: fileData, 
        }).then(response => {
            response.text().then(data => {
                if (response.ok) {
                    messageContainer.className = 'alert success';
                    message.textContent = data;
                    spiner.style.display = "none";
                    setTimeout(() => {
                        location.reload(true);
                    }, 1000);
                } else {
                    console.error(`OTA Update failed: ${response.status} ${data}`)
                    messageContainer.className = 'alert error';
                    message.textContent = `OTA Update failed: ${response.status} ${data}`;
                    spiner.style.display = "none";
                }
            })
        }).catch(error => {
            console.error('Error during OTA Update:', error);
            messageContainer.className = 'alert error';
            message.textContent = 'Error during OTA Update. See console for details.';
            spiner.style.display = "none";
        });
    });
});

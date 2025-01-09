
document.getElementById('otaForm').addEventListener('submit', async (event) => {
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

    try {
        const fileData = await file.arrayBuffer();

        const response = await fetch('/ota_update', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/octet-stream',
            },
            body: fileData, 
        });

        const alertMessage = document.getElementById('alertMessage');
        if (response.ok) {
            alertMessage.textContent = 'OTA Update successful!';
            alertMessage.className = 'alert success';
            alertMessage.style.display = 'block';
        } else {
            const errorMessage = await response.text();
            alertMessage.textContent = `OTA Update failed: ${response.status} ${errorMessage}`;
            alertMessage.className = 'alert error';
            alertMessage.style.display = 'block';
        }
    } catch (error) {
        console.error('Error during OTA Update:', error);
        const alertMessage = document.getElementById('alertMessage');
        alertMessage.textContent = 'Error during OTA Update. See console for details.';
        alertMessage.className = 'alert error';
        alertMessage.style.display = 'block';
    }
});

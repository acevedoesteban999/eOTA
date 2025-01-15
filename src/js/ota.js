const alertMessage = document.getElementById('alertMessage');
var timeout;

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

    const fileData = file.arrayBuffer();
    
    fetch('/ota_update', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/octet-stream',
        },
        body: fileData, 
    }).then(response => {
        clearTimeout(timeout);
        if (response.ok) {
            alertMessage.textContent = 'OTA Update successful! Rebooting ...';
            alertMessage.className = 'alert success';
            alertMessage.style.display = 'block';
        } else {
            response.text().then(errorMessage => {
                console.error(`OTA Update failed: ${response.status} ${errorMessage}`)
                alertMessage.textContent = `OTA Update failed: ${response.status} ${errorMessage}`;
                alertMessage.className = 'alert error';
                alertMessage.style.display = 'block';
            });
        }
    }).catch(error => {
        clearTimeout(timeout);
        console.error('Error during OTA Update:', error);
        alertMessage.textContent = 'Error during OTA Update. See console for details.';
        alertMessage.className = 'alert error';
        alertMessage.style.display = 'block';
    });

    timeout = setTimeout(checkOtaStatus,1);


});

function checkOtaStatus() {
    fetch('/ota_status', {
        method: 'POST'
    })
    .then(response => {
        response.text().then(data => {
            if(response.ok){
                const params = new URLSearchParams(data);
                const status = params.get('status');  
                const value = params.get('value');    
                let statusMessage = '';

                if (status === '0')
                    statusMessage = 'Wait OTA progress';
                else if (status === '1')
                    statusMessage = 'Getting partition...';
                else if (status === '2')
                    statusMessage = 'OTA process started...';
                else if (status === '3')
                    statusMessage = `Receiving OTA data... ${value}% completed`;
                else if (status === '4')
                    statusMessage = 'OTA process ended, preparing boot partition...';
                else if (status === '5')
                    statusMessage = 'Setting boot partition...';
                else if (status === '6')
                    statusMessage = 'OTA update successful, rebooting...';
                else
                    statusMessage = 'Unknown status';
                
                console.log('OTA status:', status, 'value:', value);
                alertMessage.textContent = statusMessage;
                alertMessage.className = 'alert warning';
                alertMessage.style.display = 'block';
            }
            else
                console.error(`OTA Update Status failed: ${response.status} ${data}`)
        })
    }) 
    .catch(error => {
        console.error('Error fetching OTA status:', error);
    });
    timeout = setTimeout(checkOtaStatus,1000);
}

const messageFile = document.getElementById("messageFileID");
const buttonSubmit = document.getElementById("buttonSubmitID");
const inputFile = document.getElementById("inputFileID");
const dropZone = document.getElementById("dropZoneID");
const msid = document.getElementById("msid");
const mbtid = document.getElementById("mbtid");

dropZone.addEventListener("click", () => {
  inputFile.click();
});

dropZone.addEventListener("dragover", (event) => {
  event.preventDefault();
  dropZone.style.borderColor = "#00bfff";
});

dropZone.addEventListener("dragleave", () => {
  dropZone.style.borderColor = "#ccc";
});

dropZone.addEventListener("drop", (event) => {
  event.preventDefault();
  console.log(event.dataTransfer.files.length);
  if (event.dataTransfer.files.length > 0) {
    file = event.dataTransfer.files[0];
    dataTransfer = new DataTransfer();
    dataTransfer.items.add(file);
    inputFile.files = dataTransfer.files;
    selectFile(file);
  }
});

inputFile.addEventListener("change", (event) => {
  selectFile(event.target.files[0]);
});

function selectFile(file) {
  messageFile.innerHTML =
    "<span style='color: black !important'>File: </span>" + file.name;
  if (file.name.endsWith(".bin")) {
    messageFile.style.color = "green";
    dropZone.style.borderColor = "green";
    buttonSubmit.disabled = false;
  } else {
    messageFile.style.color = "red";
    dropZone.style.borderColor = "red";
    buttonSubmit.disabled = true;
  }
}

document.getElementById("otaForm").addEventListener("submit", (event) => {
  event.preventDefault();

  file = inputFile.files[0];

  if (!file) {
    alert("Please select a firmware file!");
    return;
  }

  if (!file.name.endsWith(".bin")) {
    alert("Please select a .bin file!");
    return;
  }
  s(msid);
  h(mbtid);
  sM("", "");
  file.arrayBuffer().then((fileData) => {
    fetch("/ota_update", {
      method: "POST",
      headers: {
        "Content-Type": "application/octet-stream",
      },
      body: fileData,
    })
      .then((response) => {
        if (!response.ok)
          throw new Error(response.status + " " + response.statusText);
        else return response.text();
      })
      .then((data) => {
        h(msid);
        sM("h4 bd bd-ss my-3", data);
        setTimeout(() => {
          window.location.href = "./";
        }, 1000);
      })
      .catch((error) => {
        h(msid);
        s(mbtid);
        sM("h4 bd bd-dg my-3", error);
      });
  });
});

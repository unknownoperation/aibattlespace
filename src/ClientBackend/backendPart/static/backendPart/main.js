                         
var graphics = new Graphics2d();

function main() {
    graphics.main();
    renderloop();
    window.addEventListener('resize', resizeCanvas, false);
}

function renderloop() {
    graphics.main();
    window.setTimeout(this.renderloop, 1000/60);  
}

function resizeCanvas() {
    graphics.resizeDisplay();
}

function Complete() {
    playerName = document.Send_Data.playerName.value;
    if (playerName == "") {
        alert("Please enter your name")
        return;
    }

    if(document.getElementById("file").files.length == 0) {
        alert("Attach source code of your AI")
        return;
    }
    SourceCode = document.getElementById("file").files[0];

    var gotData = "Player name: " + playerName +
        "\nFile name: " + SourceCode.name;
    alert(gotData);

    // Fill sending data
    var formData = new FormData();
    formData.append('AIsource', SourceCode, playerName);

    // Make request
    var request = new XMLHttpRequest();
    request.open("POST", "uploadFile", false);
    request.send(formData);

    // Show result
    alert(request.responseText);
}


